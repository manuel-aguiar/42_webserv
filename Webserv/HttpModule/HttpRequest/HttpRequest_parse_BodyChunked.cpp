
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

// Helper function to convert string to integer, takes a base
static int strToInteger(const BufferView& view, int base = 10)
{
    char* endptr = const_cast<char *>(&view.data()[view.size()]);
    long value = ::strtol(view.data(), &endptr, base);
    std::cout << "hex value: " << value << std::endl;
    if (endptr != &view.data()[view.size()]
    || value > INT_MAX || value < 0
    || (size_t)value > Http::HttpStandard::MAX_CHUNK_SIZE)
        return (-1);

    return (static_cast<int>(value));
}


BufferView Http::Request::mf_parseChunkedBody(BaseBuffer& buffer, const BufferView& receivedView)
{
    BufferView currentView = receivedView;

    std::cout << "chunked body, current view: '" << currentView << "'" << std::endl;
	while (currentView.size() > 0 && m_parsingState == BODY)
    {
        std::cout << "entered body loop, view: '" << currentView <<  "'" << std::endl;
        if (m_curChunkSize == -1)
        {
            size_t headerEnd = currentView.find("\r\n", 2, 0);
            if (headerEnd == BufferView::npos)
            {
                // HARD LIMIT, single header size cannot be bigger than the buffer capacity
                if (currentView.size() >= buffer.capacity())
                    goto exitFailure;
                else
                    buffer.truncatePush(currentView); // push the remaining data back to the beginning
                
                return (BufferView()); // not enough to go through yet
            }
            BufferView thisChunkSize = currentView.substr(0, headerEnd);
            currentView = currentView.substr(headerEnd + 2, currentView.size() - headerEnd - 2); // move view past the header

            m_curChunkSize = strToInteger(thisChunkSize, 16);
            std::cout << "current chunk size: " << m_curChunkSize << std::endl;
            std::cout << "view size " << currentView.size() << " after chunk size: '" << currentView << "'" << std::endl;
            if (m_curChunkSize == -1)
                goto exitFailure;
            m_curChunkPos = 0;
        }
        else if (m_curChunkSize == 0)
        {

            size_t bodyEnd = currentView.find("\r\n", 2, 0);
            if (bodyEnd == BufferView::npos)
            {
                // HARD LIMIT, single header size cannot be bigger than the buffer capacity
                if (currentView.size() >= buffer.capacity())
                    goto exitFailure;
                else
                    buffer.truncatePush(currentView); // push the remaining data back to the beginning
                
                return (BufferView()); // not enough to go through yet
            }
            currentView = currentView.substr(bodyEnd + 2, currentView.size() - bodyEnd - 2); // move view past the header

            if (bodyEnd != 0)
                goto exitFailure;


            buffer.truncatePush(currentView); // push the remaining data back to the beginning

            m_parsingState = COMPLETED;
            if (m_response)
                m_response->receiveRequestBody(BufferView()); // send "eof" to signal end of body

            return (BufferView());
        }
        else
        {
            if (m_curChunkPos == m_curChunkSize)
            {
                size_t bodyEnd = currentView.find("\r\n", 2, 0);
                if (bodyEnd == BufferView::npos)
                {
                    // HARD LIMIT, single header size cannot be bigger than the buffer capacity
                    if (currentView.size() >= buffer.capacity())
                        goto exitFailure;
                    else
                        buffer.truncatePush(currentView); // push the remaining data back to the beginning
                    
                    return (BufferView()); // not enough to go through yet
                }
                currentView = currentView.substr(bodyEnd + 2, currentView.size() - bodyEnd - 2); // move view past the header
    
                if (bodyEnd != 0)
                    goto exitFailure;

                m_curChunkSize = -1;
                m_curChunkPos = 0;
            }

            // stream
            size_t bytesLeft = m_curChunkSize - m_curChunkPos;
            size_t bytesSending = (bytesLeft > currentView.size()) ? currentView.size() : bytesLeft;
        
            BufferView remaining = currentView.substr(bytesSending, currentView.size() - bytesSending);
            m_curChunkPos += bytesSending;
        
            if (m_response)
                m_response->receiveRequestBody(currentView.substr(0, bytesSending));
            
            currentView = remaining;
        }
    }

    std::cout << "exited body loop" << std::endl;

    return (currentView);

exitFailure:
    m_parsingState = ERROR;
    m_data.status = Http::Status::BAD_REQUEST;      // chunk header too long
    m_parsingFunction = &Request::mf_handleNothing;
    if (m_response)
    {
        m_response->receiveRequestBody(BufferView()); // send "eof" to signal end of body
    }
    return (BufferView());
}