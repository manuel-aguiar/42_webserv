/* *********************************/
/*                                 */
/*   http_parse_message_body.cpp   */
/*   - the message body parser for */
/*    an incoming http request.    */
/*                                 */
/* *********************************/

// Project headers
#include "HttpRequest.hpp"
#include "../HttpResponse/HttpResponse.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"

// C++ headers
#include <sstream>
#include <cstdlib>
#include <climits>

static const BufferView contentDispositionFind			("Content-Disposition");
static const BufferView contentDispositionNameFind		("name=\"");
static const BufferView contentDispositionFilenameFind	("filename=\"");

#ifndef NDEBUG
	static int testHeadersOfInterest();
	static const int g_testHeadersOfInterest = testHeadersOfInterest();
	static int testHeadersOfInterest()
	{
		std::string test = contentDispositionFind.to_string();
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToCapitalized() 
			== contentDispositionFind, true, "contentDispositionFind is not correctly formated");
		test = contentDispositionNameFind.to_string();
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToLowerCase() 
			== contentDispositionNameFind, true, "contentDispositionNameFind is not correctly formated");	
		test = contentDispositionFilenameFind.to_string();
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToLowerCase() 
			== contentDispositionFilenameFind, true, "contentDispositionFilenameFind is not correctly formated");
		return (0);
	}
#endif


/*
	@returns: BufferView of the remaining data that wasn't consumed
*/
BufferView Http::Request::mf_parseMultipartBody_Start	(const BufferView& currentView)
{
	const BufferView delimiter("\r\n", 2);
	BufferView remaining = currentView;
	BufferView requestLine;
	const BufferView boundaryView(m_data.multipart_Boundary);

	if (remaining.size() <= boundaryView.size() + delimiter.size())
		return (remaining);

	size_t reqLineEnd = remaining.find(delimiter, m_findPivot);
	if (reqLineEnd == BufferView::npos)
	{
		m_findPivot = std::max((int)remaining.size() - (int)delimiter.size(), 0);
		// HARD LIMIT, request line cannot be bigger than buffer size
		if (remaining.size() >= m_readBuffer->capacity())
			return (mf_bodyExitError(remaining, Http::Status::PAYLOAD_TOO_LARGE));
		return (remaining); // not enough to go through yet
	}
	m_findPivot = 0;

	requestLine = BufferView(remaining.substr(0, reqLineEnd));
	remaining = remaining.substr(reqLineEnd + delimiter.size(), remaining.size() - reqLineEnd - delimiter.size()); // move view forward


	m_curContentPos += requestLine.size() + delimiter.size();

	if (m_curContentPos > m_curContentLength)
		return (mf_bodyExitError(remaining, Http::Status::PAYLOAD_TOO_LARGE));

	if (requestLine.size() != boundaryView.size() + 2) // must be the size of boundary + "--"
		return (mf_bodyExitError(remaining, Http::Status::BAD_REQUEST));

	if (requestLine.find("--", 2, 0) != 0) // must start with this
		return (mf_bodyExitError(remaining, Http::Status::BAD_REQUEST));

	if (requestLine.find(boundaryView, 0) != 2) // must have boundary at position 2
		return (mf_bodyExitError(remaining, Http::Status::BAD_REQUEST));

		// success, move to next pointer
	m_parsingFunction = &Request::mf_parseMultipartBody_Headers;
	return (mf_parseMultipartBody_Headers(remaining));
}

bool partHasBody(const BufferView& part, const BufferView& tail, const BufferView& boundary)
{
	size_t findTail = part.find(tail);
	if (findTail != 0)
		return (true);
	size_t findBoundary = part.find(boundary, tail.size());
	if (findBoundary != tail.size())
		return (true);
	return (false);
}

/*
	@returns: BufferView of the remaining data that wasn't consumed
*/
BufferView Http::Request::mf_parseMultipartBody_Headers	(const BufferView& currentView)
{
	const BufferView delimiter("\r\n", 2);
	BufferView remaining = currentView;

	if (remaining.size() <= delimiter.size())
		return (remaining); // not enough to go through yet
	while (remaining.size() > 0)
	{
		size_t headerEnd = remaining.find(delimiter, m_findPivot);
		if (headerEnd == BufferView::npos)
		{
			m_findPivot = std::max((int)remaining.size() - (int)delimiter.size(), 0);
			// HARD LIMIT, single header size cannot be bigger than the buffer capacity
			if (remaining.size() >= m_readBuffer->capacity())
				return (mf_bodyExitError(remaining, Http::Status::PAYLOAD_TOO_LARGE));
			return (remaining); // push the remaining data back to the beginning

		}

		m_findPivot = 0;


		if (headerEnd == 0)
		{
			remaining = remaining.substr(delimiter.size(), remaining.size() - delimiter.size()); // move to body

			m_curContentPos += delimiter.size();	//header delimiters in multi part count towards content length

			if (m_curContentPos > m_curContentLength ||
				(m_data.multipart_Name.empty() && m_data.multipart_Filename.empty()))
			{
				return (mf_bodyExitError(remaining, Http::Status::BAD_REQUEST));
			}

			
			m_parsingFunction = &Request::mf_parseMultipartBody_Check;
			return (mf_parseMultipartBody_Check(remaining));
		}
		BufferView thisHeader = remaining.substr(0, headerEnd).trim(" \r\v\t\n"); // segregate this header


		remaining = remaining.substr(headerEnd + delimiter.size(), remaining.size() - headerEnd - delimiter.size()); // move to next header
		m_curContentPos += (thisHeader.size() + delimiter.size());

		if (m_curContentPos > m_curContentLength)
			return (mf_bodyExitError(remaining, Http::Status::PAYLOAD_TOO_LARGE));

		size_t keyPos = thisHeader.find(": ");
		if (keyPos == BufferView::npos)
			return (mf_bodyExitError(remaining, Http::Status::BAD_REQUEST));

		BufferView key = thisHeader.substr(0, keyPos).trim(" \r\v\t\n").modify_ToCapitalized();

		if (key != contentDispositionFind)
			continue ;
		{	// getting the "name" variable
			BufferView value = thisHeader.substr(keyPos + 2, thisHeader.size() - keyPos - 2);
			size_t nameStart = value.find(contentDispositionNameFind);
			if (nameStart != BufferView::npos)
			{
				value = value.substr(nameStart 
									+ contentDispositionNameFind.size(), 
									value.size() - nameStart - contentDispositionNameFind.size());
				size_t nameEnd = value.find('\"');
				if (nameEnd != BufferView::npos)
					m_data.multipart_Name = value.substr(0, nameEnd).to_string();
			}
		}

		{	// getting the "filename" variable
			BufferView value = thisHeader.substr(keyPos + 2, thisHeader.size() - keyPos - 2);
			size_t nameStart = value.find(contentDispositionFilenameFind);
			if (nameStart != BufferView::npos)
			{
				value = value.substr(nameStart + 
						contentDispositionFilenameFind.size(), 
						value.size() - nameStart - contentDispositionFilenameFind.size());
				size_t nameEnd = value.find('\"');
				if (nameEnd != BufferView::npos)
					m_data.multipart_Filename = value.substr(0, nameEnd).to_string();
			}
		}
	}

	return (remaining);
}

BufferView
Http::Request::mf_parseMultipartBody_Check(const BufferView& receivedView)
{
	BufferView remaining = receivedView;
	const BufferView	noBodyDelimiter("--", 2);
	const BufferView	boundary = BufferView(m_data.multipart_Boundary);

	if (remaining.size() <= noBodyDelimiter.size() + boundary.size())
		return (remaining);

	if (partHasBody(remaining, noBodyDelimiter, BufferView(m_data.multipart_Boundary)))
	{
		m_parsingFunction = &Request::mf_parseMultipartBody_Content;
		return (mf_parseMultipartBody_Content(remaining));
	}

	size_t moveForward = noBodyDelimiter.size() + boundary.size();
	remaining = remaining.substr(moveForward, remaining.size() - moveForward);
	m_curContentPos += moveForward;
	if (m_curContentPos > m_curContentLength)
		return (mf_bodyExitError(remaining, Http::Status::PAYLOAD_TOO_LARGE));
		
	if (m_httpResponse)	// send EOF to signal end of part
		m_httpResponse->receiveRequestBody(BufferView());

	m_parsingFunction = &Request::mf_parseMultipartBody_End;
	return (mf_parseMultipartBody_End(remaining));
}

static size_t
findDelimiterAndBoundary (const BufferView& haystack, const BufferView& delimiter, const BufferView& boundary)
{
	size_t delimiterPos = haystack.find(delimiter);
	if (delimiterPos == BufferView::npos)
		return (BufferView::npos);
	size_t boundaryPos = haystack.find(boundary, delimiterPos);
	if (boundaryPos == BufferView::npos)
		return (BufferView::npos);
	return (delimiterPos);
}

/*
	@returns: BufferView of the remaining data that wasn't consumed
*/




BufferView Http::Request::mf_parseMultipartBody_Content	(const BufferView& currentView)
{
	const BufferView	bodyDelimiter("\r\n--", 4);
	const BufferView    boundary = BufferView(m_data.multipart_Boundary);
	BufferView	remaining = currentView;
	BufferView	unconsumed;
	size_t 		moveForward = m_data.multipart_Boundary.size() + bodyDelimiter.size();
	size_t 		chunkEnd;

	if (remaining.size() <= moveForward)
		return (remaining);

	size_t doubleHifenBoundary = findDelimiterAndBoundary(remaining, bodyDelimiter, BufferView(m_data.multipart_Boundary));
	if (doubleHifenBoundary == BufferView::npos)
		chunkEnd = std::max((int)remaining.size() - (int)moveForward, (int)0);
	else
		chunkEnd = doubleHifenBoundary;

	if (m_curContentPos + (int)chunkEnd > m_curContentLength)
		return (mf_bodyExitError(remaining, Http::Status::PAYLOAD_TOO_LARGE));

	if (m_httpResponse && chunkEnd > 0)
        unconsumed = m_httpResponse->receiveRequestBody(currentView.substr(0, chunkEnd));

	size_t bytesConsumed = chunkEnd - unconsumed.size();

	remaining = remaining.substr(bytesConsumed, remaining.size() - bytesConsumed);

	m_curContentPos += bytesConsumed;

	if (bytesConsumed == doubleHifenBoundary)
	{
		remaining = remaining.substr(moveForward, remaining.size() - moveForward);
		m_curContentPos += moveForward;
		if (m_curContentPos > m_curContentLength)
			return (mf_bodyExitError(remaining, Http::Status::PAYLOAD_TOO_LARGE));
		
		// this is a file, send end of file here
		if (m_httpResponse)
			m_httpResponse->receiveRequestBody(BufferView());

		m_parsingFunction = &Request::mf_parseMultipartBody_End;
		return (mf_parseMultipartBody_End(remaining));
	}

	return (remaining);
}

/*
	@returns: BufferView of the remaining data that wasn't consumed
*/
BufferView 			Http::Request::mf_parseMultipartBody_End		(const BufferView& currentView)
{
	BufferView remaining = currentView;
	BufferView found;
	const BufferView continueDelim("\r\n", 2);
	const BufferView finalDelim("--\r\n", 4);

	if (remaining.size() < finalDelim.size())
		return (remaining);
	if (remaining.find(finalDelim, 0) == 0)
	{
		found = finalDelim;
		m_findPivot = 0;
		m_parsingState = COMPLETED;
		m_parsingFunction = &Request::mf_handleNothing;

		if (m_curContentPos + found.size() != (size_t)m_curContentLength)
			return (mf_bodyExitError(remaining, Http::Status::BAD_REQUEST));
			
		m_data.multipart_Name.clear();
		m_data.multipart_Filename.clear();
			
		// double signaling, one for end of this file, below to end of request
		if (m_httpResponse)
			m_httpResponse->receiveRequestBody(BufferView());

		m_parsingState = COMPLETED;
	}
	else if (remaining.find(continueDelim, 0) == 0)
	{
		found = continueDelim;
		m_findPivot = 0;
		m_parsingFunction = &Request::mf_parseMultipartBody_Headers;
	}
	else
		return (mf_bodyExitError(remaining, Http::Status::BAD_REQUEST));
	
	remaining = remaining.substr(found.size(), remaining.size() - found.size());
	m_curContentPos += found.size();
	m_data.multipart_Name.clear();
	m_data.multipart_Filename.clear();

	return ((this->*m_parsingFunction)(remaining));
}


