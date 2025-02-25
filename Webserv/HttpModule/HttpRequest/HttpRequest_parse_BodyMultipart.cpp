
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


BufferView Http::Request::mf_parseMultipartBody_Start	(const BaseBuffer& buffer, const BufferView& currentView)
{
	BufferView remaining = currentView;

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
			if (remaining.size() >= buffer.capacity())
			{
				m_parsingState = ERROR;
				m_data.status = Http::Status::BAD_REQUEST;      // URI too long, AMMEND ERROR CODE
				m_response->receiveRequestData(m_data);         // inform response right away
			}
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
	
	BufferView requestLine(remaining.substr(0, reqLineEnd));
	remaining = remaining.substr(reqLineEnd + 2, remaining.size() - reqLineEnd - 2); // move view forward
	
	BufferView boundaryView(m_data.boundary);
	
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
	return (mf_parseMultipartBody_Headers(buffer, remaining));

exitFailure:
	m_parsingState = ERROR;
	if (m_response)
		m_response->receiveRequestBody(BufferView());                 // send empty body to response

	return (remaining);
}

BufferView Http::Request::mf_parseMultipartBody_Headers	(const BaseBuffer& buffer, const BufferView& currentView)
{
	(void)buffer;
	(void)currentView;
	return (BufferView());
}

BufferView Http::Request::mf_parseMultipartBody_Content	(const BaseBuffer& buffer, const BufferView& currentView)
{
	(void)buffer;
	(void)currentView;
	return (BufferView());
}

BufferView Http::Request::mf_parseMultipartBody_End		(const BaseBuffer& buffer, const BufferView& currentView)
{
	(void)buffer;
	(void)currentView;
	return (BufferView());
}
