
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

/*
	@returns: BufferView of the remaining data that wasn't consumed
*/
BufferView Http::Request::mf_parseLengthBody(const BufferView& currentView)
{
    size_t      bytesLeft = m_curContentLength - m_curContentPos;
    size_t      bytesSending = (bytesLeft > currentView.size()) ? currentView.size() : bytesLeft;
    BufferView  unconsumed;

    if (m_httpResponse && bytesSending > 0)
        unconsumed = m_httpResponse->receiveRequestBody(currentView.substr(0, bytesSending));

    size_t      bytesConsumed = bytesSending - unconsumed.size();

    BufferView  remaining = currentView.substr(bytesConsumed, currentView.size() - bytesConsumed);
    m_curContentPos += bytesConsumed;
    
    if (m_curContentPos == m_curContentLength)
    {
        m_parsingState = COMPLETED;
        if (m_httpResponse)
            m_httpResponse->receiveRequestBody(BufferView()); // send empty body, signals end of body
    }

    return (remaining);
}
