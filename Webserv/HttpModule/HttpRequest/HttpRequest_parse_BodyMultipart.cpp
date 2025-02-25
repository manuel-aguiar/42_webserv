
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

// C++ headers
#include <sstream>
#include <cstdlib>
#include <climits>

// always try to find the separator, dont parse more than view remainingView.size() - boundary size to avoid catching it
//
// start of body must be equal to boundary, skip
//
//


BufferView Http::Request::mf_parseMultipartBody_Start	(const BufferView& currentView)
{
	BufferView remaining = currentView;
	BufferView requestLine;
	BufferView boundaryView;

	if (remaining.size() == 0)
		return (BufferView());
	
	size_t reqLineEnd;
	
	while (1)
	{
		reqLineEnd = remaining.find('\r', m_findPivot);
		if (reqLineEnd == BufferView::npos || reqLineEnd == remaining.size() - 1)
		{
			m_findPivot = std::max((int)remaining.size() - 1, 0);
			// HARD LIMIT, request line cannot be bigger than buffer size
			if (remaining.size() >= m_bufferCapacity)
				goto exitFailure;
			return (remaining); // not enough to go through yet
		}
		else if (remaining[reqLineEnd + 1] != '\n')
		{
			////std::cout << "found in the middle of a header" << std::endl;
			m_findPivot = reqLineEnd + 1;
		}
		else
		{
			m_findPivot = 0;
			break ;
		}
	}
	
	requestLine = BufferView(remaining.substr(0, reqLineEnd));
	remaining = remaining.substr(reqLineEnd + 2, remaining.size() - reqLineEnd - 2); // move view forward
	
	boundaryView = BufferView(m_data.multipart_Boundary);
	
	m_curContentPos += requestLine.size();

	if (m_curContentPos > m_curContentLength)
		goto exitFailure;

	if (requestLine.size() != boundaryView.size() + 2) // must be the size of boundary + "--"
		goto exitFailure;

	if (requestLine.find("--", 2, 0) != 0) // must start with this
		goto exitFailure;

	if (requestLine.find(boundaryView, 0) != 2) // must have boundary at position 2
		goto exitFailure;

	
	// success, move to next pointer
	m_parsingFunction = &Request::mf_parseMultipartBody_Headers;
	return (mf_parseMultipartBody_Headers(remaining));

exitFailure:
	m_data.status = Http::Status::BAD_REQUEST;      				// Something went wrong, size or something
	m_parsingState = ERROR;
	if (m_response)
		m_response->receiveRequestBody(BufferView());                 // send empty body to response
	m_parsingFunction = &Request::mf_handleNothing;
	return (remaining);
}

BufferView Http::Request::mf_parseMultipartBody_Headers	(const BufferView& currentView)
{
	BufferView remaining = currentView;

	if (remaining.size() == 0)
		return (remaining); // not enough to go through yet

	while (remaining.size() > 0)
	{
		//std::cout << "\t\t multipar headers in loop " << remaining.size() << ", view: ->" << remaining << "<->" << std::endl;

		////std::cout << "lookup pivot: " << m_findPivot << std::endl;
		size_t headerEnd = remaining.find('\r', m_findPivot);
		if (headerEnd == BufferView::npos || headerEnd == remaining.size() - 1)
		{
			m_findPivot = std::max((int)remaining.size() - 1, 0);
			// HARD LIMIT, single header size cannot be bigger than the buffer capacity
			if (remaining.size() >= m_bufferCapacity)
				goto exitFailure;
			return (remaining); // push the remaining data back to the beginning
			
		}
		else if (remaining[headerEnd + 1] != '\n')
		{
			////std::cout << "found in the middle of a header" << std::endl;
			m_findPivot = headerEnd + 1;
			continue ;
		}
		else
			m_findPivot = 0;

		////std::cout << "header end: " << headerEnd << std::endl;

		if (headerEnd == 0)
		{
			//std::cout << " found end of headers" << std::endl;
			// \r\n found at the beginning: end of headers, move to BODY
			remaining = remaining.substr(2, remaining.size() - 2); // move to body
			if (m_data.multipart_Name.empty() || m_data.multipart_Filename.empty())
				goto exitFailure;

			m_parsingFunction = &Request::mf_parseMultipartBody_Content;
			return (mf_parseMultipartBody_Content(remaining));
		}
		BufferView thisHeader = remaining.substr(0, headerEnd); // segregate this header
		
		//std::cout << "\t\t\t\theader: ->" << thisHeader << "<-" << std::endl;

		remaining = remaining.substr(headerEnd + 2, remaining.size() - headerEnd - 2); // move to next header
		m_curContentPos += (thisHeader.size() + 2);

		//std::cout << "m_curContentPos: " << m_curContentPos << ", m_curContentLength: " << m_curContentLength << std::endl;
		if (m_curContentPos > m_curContentLength)
			goto exitFailure;
		//std::cout << "\t\t\t\t\tmax size okay" << std::endl;
		size_t keyPos = thisHeader.find(": ");
		if (keyPos == BufferView::npos)
			goto exitFailure;
		//std::cout << "\t\t\t\t\tcoma okay" << std::endl;
		BufferView key = thisHeader.substr(0, keyPos);
		if (key != BufferView("Content-Disposition"))
			continue ;
		
		//std::cout << "\t\t\t\t\tcontent-disposition found" << std::endl;

		{	// getting the "name" variable
			BufferView value = thisHeader.substr(keyPos + 2, thisHeader.size() - keyPos - 2);
			size_t nameStart = value.find("name=\"");
			if (nameStart == BufferView::npos)
				goto exitFailure;
			value = value.substr(nameStart + 6, value.size() - nameStart - 6);
			size_t nameEnd = value.find("\"");
			if (nameEnd == BufferView::npos)
				goto exitFailure;
			m_data.multipart_Name = value.substr(0, nameEnd).to_string();			
		}

		{	// getting the "filename" variable
			BufferView value = thisHeader.substr(keyPos + 2, thisHeader.size() - keyPos - 2);
			size_t nameStart = value.find("filename=\"");
			if (nameStart == BufferView::npos)
				goto exitFailure;
			value = value.substr(nameStart + 10, value.size() - nameStart - 10);
			size_t nameEnd = value.find("\"");
			if (nameEnd == BufferView::npos)
				goto exitFailure;
			m_data.multipart_Filename = value.substr(0, nameEnd).to_string();			
		}
		//std::cout << "\t\t\t\t\t all good, name: " << m_data.multipart_Name << ", filename: " << m_data.multipart_Filename << std::endl;
		//std::cout << "\t\t\t\t\tremaining: ->" << remaining << "<-" << std::endl;
	}

	return (remaining);

exitFailure:
	m_data.status = Http::Status::BAD_REQUEST;      				// Something went wrong, size or something
	m_parsingState = ERROR;
	if (m_response)
		m_response->receiveRequestBody(BufferView());                 // send empty body to response
	m_parsingFunction = &Request::mf_handleNothing;
	return (remaining);
}

BufferView Http::Request::mf_parseMultipartBody_Content	(const BufferView& currentView)
{
	//std::cout << "body content called,  name: " << m_data.multipart_Name << ", filename: " << m_data.multipart_Filename << std::endl;
	//std::cout << "current view: ->" << currentView << "<-" << std::endl;
	BufferView remaining = currentView;
	size_t chunkEnd;

	if (remaining.size() < m_data.multipart_Boundary.size() + 2) // can't checkout "--" + boundary, may save accidentallys
		return (remaining);
	
	size_t doubleHifenBoundary = mf_parseMultipart_FoundBoundary(remaining);

	if (doubleHifenBoundary == BufferView::npos)
	{
		//std::cout << "separator not found"  << std::endl;
		chunkEnd = std::max(remaining.size() - m_data.multipart_Boundary.size() - 2, (size_t)0);
	}
	else
	{
		//std::cout << "separator found"  << std::endl;
		chunkEnd = doubleHifenBoundary;
	}
	
	//std::cout << "\t\t\tsending: ->" << remaining.substr(0, chunkEnd) << "<-" << std::endl;
	
	remaining = remaining.substr(chunkEnd, remaining.size() - chunkEnd);

	m_curContentPos += chunkEnd;
	if (m_curContentPos > m_curContentLength)
		goto exitFailure;

	if (m_response)
        m_response->receiveRequestBody(currentView.substr(0, chunkEnd));

	if (chunkEnd == doubleHifenBoundary)
	{
		//std::cout << "reached boundary" << std::endl;
		// move further "\r\n--" and boundary
		size_t moveForward = 4 + m_data.multipart_Boundary.size();
		remaining = remaining.substr(moveForward, remaining.size() - moveForward);
		m_curContentPos += moveForward;
		if (m_curContentPos > m_curContentLength)
			goto exitFailure;
		//std::cout << "before end view: ->" << remaining << "<-" << std::endl;
		m_parsingFunction = &Request::mf_parseMultipartBody_End;
	}

	return ((this->*m_parsingFunction)(remaining));

exitFailure:
	m_data.status = Http::Status::BAD_REQUEST;      				// Something went wrong, size or something
	m_parsingState = ERROR;
	if (m_response)
		m_response->receiveRequestBody(BufferView());                 // send empty body to response
	m_parsingFunction = &Request::mf_handleNothing;
	return (mf_handleNothing(remaining));
}


BufferView 			Http::Request::mf_parseMultipartBody_End		(const BufferView& currentView)
{
	BufferView requestLine;
	BufferView remaining = currentView;

	if (currentView.size() == 0)
		return (currentView);
	
	size_t reqLineEnd;

	while (1)
	{
		reqLineEnd = remaining.find('\r', m_findPivot);
		if (reqLineEnd == BufferView::npos || reqLineEnd == remaining.size() - 1)
		{
			m_findPivot = std::max((int)remaining.size() - 1, 0);
			// HARD LIMIT, request line cannot be bigger than buffer size
			if (remaining.size() >= m_bufferCapacity)
				goto exitFailure;
			return (remaining); // not enough to go through yet
		}
		else if (remaining[reqLineEnd + 1] != '\n')
		{
			////std::cout << "found in the middle of a header" << std::endl;
			m_findPivot = reqLineEnd + 1;
		}
		else
		{
			m_findPivot = 0;
			break ;
		}
	}
	
	requestLine = BufferView(remaining.substr(0, reqLineEnd));
	remaining = remaining.substr(reqLineEnd + 2, remaining.size() - reqLineEnd - 2);

	//std::cout << "after removing \"r\"n: ->" << remaining << "<-" << std::endl;

	m_curContentPos += requestLine.size();
	if (m_curContentPos > m_curContentLength)
		goto exitFailure;
	
	if (requestLine.size() == 0)
	{
		//std::cout << "empty line, back to the beginning" << std::endl;
		// back to the beginning
		m_data.multipart_Name.clear();
		m_data.multipart_Filename.clear();
		m_parsingFunction = &Request::mf_parseMultipartBody_Headers;
	}
	else if (requestLine == BufferView("--"))
	{
		//std::cout << "current content pos: " << m_curContentPos << ", current content length: " << m_curContentLength << std::endl;
		// end of multipart
		if (m_curContentPos != m_curContentLength)
			goto exitFailure;
		m_parsingState = COMPLETED;
		m_parsingFunction = &Request::mf_handleNothing;
	}
	else
		goto exitFailure;
	
	return ((this->*m_parsingFunction)(remaining));

exitFailure:
	m_parsingState = ERROR;
	m_data.status = Http::Status::BAD_REQUEST;      				// Something went wrong, size or something
	if (m_response)
		m_response->receiveRequestBody(BufferView());                 // send empty body to response
	m_parsingFunction = &Request::mf_handleNothing;
	return (mf_handleNothing(remaining));	
	
}

size_t Http::Request::mf_parseMultipart_FoundBoundary		(const BufferView& currentView)
{

	size_t doubleHifen = currentView.find("\r\n--", 4);
	if (doubleHifen == BufferView::npos)
	{
		//std::cout << "no double hifen found" << std::endl;
		return (BufferView::npos);
	}
	size_t boundary = currentView.find(BufferView(m_data.multipart_Boundary), doubleHifen);
	if (boundary == BufferView::npos)
	{
		//std::cout << "no boundary found" << std::endl;
		return (BufferView::npos);
	}

	return (doubleHifen);
}
