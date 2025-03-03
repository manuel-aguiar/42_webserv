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

static const char* contentDispositionFind = "Content-Disposition";
static const char* contentDispositionNameFind = "name=\"";
static const char* contentDispositionFilenameFind = "filename=\"";

#ifndef NDEBUG
	int testHeadersOfInterest();
	static const int g_testHeadersOfInterest = testHeadersOfInterest();
	int testHeadersOfInterest()
	{
		std::string test = contentDispositionFind;
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToCapitalized() 
			== BufferView(contentDispositionFind), true, "contentDispositionFind is not correctly formated");
		test = contentDispositionNameFind;
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToLowerCase() 
			== BufferView(contentDispositionNameFind), true, "contentDispositionNameFind is not correctly formated");	
		test = contentDispositionFilenameFind;
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToLowerCase() 
			== BufferView(contentDispositionFilenameFind), true, "contentDispositionFilenameFind is not correctly formated");
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
	BufferView boundaryView;

	if (remaining.size() <= delimiter.size())
		return (remaining);

	size_t reqLineEnd = remaining.find(delimiter, m_findPivot);
	if (reqLineEnd == BufferView::npos)
	{
		m_findPivot = std::max((int)remaining.size() - (int)delimiter.size(), 0);
		// HARD LIMIT, request line cannot be bigger than buffer size
		if (remaining.size() >= m_bufferCapacity)
			return (mf_parseBodyExitError(Http::Status::PAYLOAD_TOO_LARGE));
		return (remaining); // not enough to go through yet
	}
	m_findPivot = 0;

	requestLine = BufferView(remaining.substr(0, reqLineEnd));
	remaining = remaining.substr(reqLineEnd + delimiter.size(), remaining.size() - reqLineEnd - delimiter.size()); // move view forward

	boundaryView = BufferView(m_data.multipart_Boundary);

	m_curContentPos += requestLine.size();

	if (m_curContentPos > m_curContentLength)
		return (mf_parseBodyExitError(Http::Status::PAYLOAD_TOO_LARGE));

	if (requestLine.size() != boundaryView.size() + 2) // must be the size of boundary + "--"
		return (mf_parseBodyExitError(Http::Status::BAD_REQUEST));

	if (requestLine.find("--", 2, 0) != 0) // must start with this
		return (mf_parseBodyExitError(Http::Status::BAD_REQUEST));

	if (requestLine.find(boundaryView, 0) != 2) // must have boundary at position 2
		return (mf_parseBodyExitError(Http::Status::BAD_REQUEST));

	// success, move to next pointer
	m_parsingFunction = &Request::mf_parseMultipartBody_Headers;
	return (mf_parseMultipartBody_Headers(remaining));
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
		//std::cout << "\t\t multipar headers in loop " << remaining.size() << ", view: ->" << remaining << "<->" << std::endl;

		////std::cout << "lookup pivot: " << m_findPivot << std::endl;
		size_t headerEnd = remaining.find(delimiter, m_findPivot);
		if (headerEnd == BufferView::npos)
		{
			m_findPivot = std::max((int)remaining.size() - (int)delimiter.size(), 0);
			// HARD LIMIT, single header size cannot be bigger than the buffer capacity
			if (remaining.size() >= m_bufferCapacity)
				return (mf_parseBodyExitError(Http::Status::PAYLOAD_TOO_LARGE));
			return (remaining); // push the remaining data back to the beginning

		}

		m_findPivot = 0;

		////std::cout << "header end: " << headerEnd << std::endl;

		if (headerEnd == 0)
		{
			//std::cout << " found end of headers" << std::endl;
			// \r\n found at the beginning: end of headers, move to BODY
			remaining = remaining.substr(delimiter.size(), remaining.size() - delimiter.size()); // move to body

			m_curContentPos += delimiter.size();	//header delimiters in multi part count towards content length

			if (m_curContentPos > m_curContentLength ||
				m_data.multipart_Name.empty() ||
				m_data.multipart_Filename.empty())

			m_curContentPos += delimiter.size();	//header delimiters in multi part count towards content length

			if (m_curContentPos > m_curContentLength ||
				m_data.multipart_Name.empty() ||
				m_data.multipart_Filename.empty())
				return (mf_parseBodyExitError(Http::Status::BAD_REQUEST));

			m_parsingFunction = &Request::mf_parseMultipartBody_Content;
			return (mf_parseMultipartBody_Content(remaining));
		}
		BufferView thisHeader = remaining.substr(0, headerEnd).trim(" \r\v\t\n"); // segregate this header

		//std::cout << "\t\t\t\theader: ->" << thisHeader << "<-" << std::endl;

		remaining = remaining.substr(headerEnd + delimiter.size(), remaining.size() - headerEnd - delimiter.size()); // move to next header
		m_curContentPos += (thisHeader.size() + delimiter.size());

		if (m_curContentPos > m_curContentLength)
			return (mf_parseBodyExitError(Http::Status::PAYLOAD_TOO_LARGE));

		size_t keyPos = thisHeader.find(": ");
		if (keyPos == BufferView::npos)
			return (mf_parseBodyExitError(Http::Status::BAD_REQUEST));

		BufferView key = thisHeader.substr(0, keyPos).trim(" \r\v\t\n").modify_ToCapitalized();

		if (key != BufferView(contentDispositionFind))
			continue ;
		
		{	// getting the "name" variable
			BufferView value = thisHeader.substr(keyPos + 2, thisHeader.size() - keyPos - 2);
			size_t nameStart = value.find(contentDispositionNameFind);
			if (nameStart == BufferView::npos)
				return (mf_parseBodyExitError(Http::Status::BAD_REQUEST));
			value = value.substr(nameStart + 6, value.size() - nameStart - 6);
			size_t nameEnd = value.find('\"');
			if (nameEnd == BufferView::npos)
				return (mf_parseBodyExitError(Http::Status::BAD_REQUEST));
			m_data.multipart_Name = value.substr(0, nameEnd).to_string();
		}

		{	// getting the "filename" variable
			BufferView value = thisHeader.substr(keyPos + 2, thisHeader.size() - keyPos - 2);
			size_t nameStart = value.find(contentDispositionFilenameFind);
			if (nameStart == BufferView::npos)
				return (mf_parseBodyExitError(Http::Status::BAD_REQUEST));
			value = value.substr(nameStart + 10, value.size() - nameStart - 10);
			size_t nameEnd = value.find('\"');
			if (nameEnd == BufferView::npos)
				return (mf_parseBodyExitError(Http::Status::BAD_REQUEST));
			m_data.multipart_Filename = value.substr(0, nameEnd).to_string();
		}
		//std::cout << "\t\t\t\t\t all good, name: " << m_data.multipart_Name << ", filename: " << m_data.multipart_Filename << std::endl;
		//std::cout << "\t\t\t\t\tremaining: ->" << remaining << "<-" << std::endl;
	}

	return (remaining);
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
	//std::cout << "body content called,  name: " << m_data.multipart_Name << ", filename: " << m_data.multipart_Filename << std::endl;
	//std::cout << "current view: ->" << currentView << "<-" << std::endl;
	const BufferView	delimiter("\r\n--", 4);
	BufferView	remaining = currentView;
	BufferView	unconsumed;
	size_t 		moveForward = m_data.multipart_Boundary.size() + delimiter.size();
	size_t 		chunkEnd;

	if (remaining.size() <= moveForward) // can't checkout "--" + boundary, may save accidentallys
		return (remaining);

	size_t doubleHifenBoundary = findDelimiterAndBoundary(remaining, delimiter, BufferView(m_data.multipart_Boundary));

	if (doubleHifenBoundary == BufferView::npos)
	{
		//std::cout << "separator not found"  << std::endl;
		chunkEnd = std::max((int)remaining.size() - (int)moveForward, (int)0);
	}
	else
	{
		//std::cout << "separator found"  << std::endl;
		chunkEnd = doubleHifenBoundary;
	}

	if (m_curContentPos + (int)chunkEnd > m_curContentLength)
		return (mf_parseBodyExitError(Http::Status::PAYLOAD_TOO_LARGE));
	//std::cout << "\t\t\tsending: ->" << remaining.substr(0, chunkEnd) << "<-" << std::endl;

	if (m_response && chunkEnd > 0)
        unconsumed = m_response->receiveRequestBody(currentView.substr(0, chunkEnd));

	size_t bytesConsumed = chunkEnd - unconsumed.size();

	remaining = remaining.substr(bytesConsumed, remaining.size() - bytesConsumed);

	m_curContentPos += bytesConsumed;

	// implies ChunkEnd == doubleHifenBoundary and all bytes consumed
	if (bytesConsumed == doubleHifenBoundary)
	{
		//std::cout << "reached boundary" << std::endl;
		// move further "\r\n--" and boundary

		remaining = remaining.substr(moveForward, remaining.size() - moveForward);
		m_curContentPos += moveForward;
		if (m_curContentPos > m_curContentLength)
			return (mf_parseBodyExitError(Http::Status::PAYLOAD_TOO_LARGE));
		//std::cout << "before end view: ->" << remaining << "<-" << std::endl;
		m_parsingFunction = &Request::mf_parseMultipartBody_End;
	}

	return ((this->*m_parsingFunction)(remaining));
}

/*
	@returns: BufferView of the remaining data that wasn't consumed
*/
BufferView 			Http::Request::mf_parseMultipartBody_End		(const BufferView& currentView)
{
	BufferView remaining = currentView;
	BufferView requestLine;
	const BufferView delimiter("\r\n", 2);
	const BufferView theEnd("--", 2);

	if (remaining.size() <= delimiter.size())
		return (remaining);

	size_t	reqLineEnd = remaining.find(delimiter, m_findPivot);
	if (reqLineEnd == BufferView::npos)
	{
		m_findPivot = std::max((int)remaining.size() - (int)delimiter.size(), 0);
		// HARD LIMIT, request line cannot be bigger than buffer size
		if (remaining.size() >= m_bufferCapacity)
			return (mf_parseBodyExitError(Http::Status::PAYLOAD_TOO_LARGE));
		return (remaining); // not enough to go through yet
	}

	m_findPivot = 0;

	requestLine = BufferView(remaining.substr(0, reqLineEnd));
	remaining = remaining.substr(reqLineEnd + delimiter.size(), remaining.size() - reqLineEnd - delimiter.size());

	//std::cout << "after removing \"r\"n: ->" << remaining << "<-" << std::endl;

	m_curContentPos += requestLine.size() + delimiter.size();

	if (m_curContentPos > m_curContentLength)
		return (mf_parseBodyExitError(Http::Status::PAYLOAD_TOO_LARGE));

	if (requestLine.size() == 0)
	{
		// end of this file upload
		//std::cout << "empty line, back to the beginning" << std::endl;
		// back to the beginning
		m_data.multipart_Name.clear();
		m_data.multipart_Filename.clear();
		m_parsingFunction = &Request::mf_parseMultipartBody_Headers;

		// send EOF to signal end of file
		//std::cout << "sending empty body" << std::endl;
		if (m_response)
			m_response->receiveRequestBody(BufferView());
	}
	else if (requestLine == theEnd)
	{
		//std::cout << "current content pos: " << m_curContentPos << ", current content length: " << m_curContentLength << std::endl;
		// end of multipart
		size_t endJump = theEnd.size() + delimiter.size();

		m_curContentPos += endJump - delimiter.size();

		if (m_curContentPos != m_curContentLength)
			return (mf_parseBodyExitError(Http::Status::BAD_REQUEST));

		m_parsingState = COMPLETED;
		m_parsingFunction = &Request::mf_handleNothing;
	}
	else
		return (mf_parseBodyExitError(Http::Status::BAD_REQUEST));

	return ((this->*m_parsingFunction)(remaining));
}


