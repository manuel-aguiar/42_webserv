
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
			//std::cout << "found in the middle of a header" << std::endl;
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
	
	boundaryView = BufferView(m_data.boundary);
	
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
		//std::cout << "\t\t current size " << remaining.size() << ", view: '" << remaining << "'" << std::endl;

		//std::cout << "lookup pivot: " << m_findPivot << std::endl;
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
			//std::cout << "found in the middle of a header" << std::endl;
			m_findPivot = headerEnd + 1;
			continue ;
		}
		else
			m_findPivot = 0;

		//std::cout << "header end: " << headerEnd << std::endl;

		if (headerEnd == 0)
		{
			// \r\n found at the beginning: end of headers, move to BODY
			remaining = remaining.substr(2, remaining.size() - 2); // move to body
			m_parsingState = BODY;
			mf_prepareBodyParser(); // next handler is body
            break ;
		}
		BufferView thisHeader = remaining.substr(0, headerEnd); // segregate this header

		remaining = remaining.substr(headerEnd + 2, remaining.size() - headerEnd - 2); // move to next header
		m_curContentPos += (thisHeader.size() + 2);

		if (m_curContentPos > m_curContentLength)
			goto exitFailure;

		size_t keyPos = thisHeader.find(": ");
		if (keyPos == BufferView::npos)
			goto exitFailure;
		
		BufferView key = thisHeader.substr(0, keyPos);
		if (key != BufferView("Content-Disposition"))
			continue ;
	}

	m_parsingFunction = &Request::mf_parseMultipartBody_Content;
	return (mf_parseMultipartBody_Content(remaining));

exitFailure:
	m_parsingState = ERROR;
	if (m_response)
		m_response->receiveRequestBody(BufferView());                 // send empty body to response
	m_parsingFunction = &Request::mf_handleNothing;
	return (remaining);

}

BufferView Http::Request::mf_parseMultipartBody_Content	(const BufferView& currentView)
{
	(void)currentView;
	return (BufferView());
}

BufferView Http::Request::mf_parseMultipartBody_End		(const BufferView& currentView)
{
	(void)currentView;
	return (BufferView());
}
