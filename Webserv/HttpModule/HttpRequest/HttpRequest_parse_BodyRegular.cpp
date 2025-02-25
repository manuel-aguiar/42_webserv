
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


BufferView Http::Request::mf_parseRegularBody(const BaseBuffer& buffer, const BufferView& currentView)
{
    (void)buffer;

    size_t bytesLeft = m_curContentLength - m_curContentPos;
    size_t bytesSending = (bytesLeft > currentView.size()) ? currentView.size() : bytesLeft;

    BufferView remaining = currentView.substr(bytesSending, currentView.size() - bytesSending);
    m_curContentPos += bytesSending;

    if (m_response)
        m_response->receiveRequestBody(currentView.substr(0, bytesSending));

    if (m_curContentPos == m_curContentLength)
    {
        m_parsingState = COMPLETED;
        if (m_response)
            m_response->receiveRequestBody(BufferView()); // send empty body, signals end of body
    }

    return (remaining);
}