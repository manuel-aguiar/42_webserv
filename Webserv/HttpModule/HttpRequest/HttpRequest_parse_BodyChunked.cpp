
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
    //std::cout << "hex value: " << value << std::endl;
    if (endptr != &view.data()[view.size()]
    || value > INT_MAX || value < 0
    || (size_t)value > Http::HttpStandard::MAX_CHUNK_SIZE)
        return (-1);

    return (static_cast<int>(value));
}

size_t Http::Request::mf_findHeaderEnd(const BufferView& remaining)
{
    size_t headerEnd = remaining.find('\r', m_findPivot);
    if (headerEnd == BufferView::npos || headerEnd == remaining.size() - 1)
    {
        m_findPivot = std::max((int)remaining.size() - 1, 0);
        return (BufferView::npos);
    }
    else if (remaining[headerEnd + 1] != '\n')
    {
        m_findPivot = headerEnd + 1;
        return (BufferView::npos);
    }
    else
        m_findPivot = 0;

    return (headerEnd);
}

/*
    @returns: the remaining view that was not consumed
*/
BufferView Http::Request::mf_parseChunkedBody(const BufferView& receivedView)
{
    BufferView remaining = receivedView;
    BufferView delimiter("\r\n", 2);

    //std::cout << "chunked body, current view: '" << remaining << "'" << std::endl;
	while (remaining.size() > 0 && m_parsingState == BODY)
    {
        //std::cout << "entered body loop, view: '" << remaining <<  "'" << std::endl;

        // no active chunk
        if (m_curChunkSize == -1)
        {
            
            size_t headerEnd = remaining.find(delimiter, m_findPivot);
            if (headerEnd == BufferView::npos || headerEnd == remaining.size() - 1)
            {
                m_findPivot = std::max((int)remaining.size() - (int)delimiter.size(), 0);

                // HARD LIMIT, single header size cannot be bigger than the buffer capacity
                if (remaining.size() >= m_bufferCapacity)
                    goto exitFailure;

                return (remaining); // not enough to go through yet
            }
            
            m_findPivot = 0;

            BufferView thisChunkSize = remaining.substr(0, headerEnd);
            remaining = remaining.substr(headerEnd + 2, remaining.size() - headerEnd - 2); // move view past the header

            m_curChunkSize = strToInteger(thisChunkSize, 16);
            if (m_curChunkSize == -1)
                goto exitFailure;
            m_curChunkPos = 0;
        }

        // last chunk
        else if (m_curChunkSize == 0)
        {

            size_t bodyEnd = remaining.find(delimiter, m_findPivot);
            if (bodyEnd == BufferView::npos || bodyEnd == remaining.size() - 1)
            {
                m_findPivot = std::max((int)remaining.size() - (int)delimiter.size(), 0);
                // HARD LIMIT, single header size cannot be bigger than the buffer capacity
                if (remaining.size() >= m_bufferCapacity)
                    goto exitFailure;
                return (remaining); // not enough to go through yet
            }

            m_findPivot = 0;

            remaining = remaining.substr(bodyEnd + 2, remaining.size() - bodyEnd - 2); // move view past the header

            if (bodyEnd != 0)
                goto exitFailure;

            m_parsingState = COMPLETED;
            if (m_response)
                m_response->receiveRequestBody(BufferView()); // send "eof" to signal end of body

            return (remaining); //return left overs, may be a followup request
        }
        else
        {
            // end of current chunk
            if (m_curChunkPos == m_curChunkSize)
            {
                size_t bodyEnd = remaining.find(delimiter, m_findPivot);
                // not found, last character in view or not followed by \n
                if (bodyEnd == BufferView::npos)
                {
                    m_findPivot = std::max((int)remaining.size() - (int)delimiter.size(), 0);
                    // HARD LIMIT, single header size cannot be bigger than the buffer capacity
                    if (remaining.size() >= m_bufferCapacity)
                        goto exitFailure;
                    return (remaining); // not enough to go through yet
                }
                
                m_findPivot = 0;
                
                remaining = remaining.substr(bodyEnd + 2, remaining.size() - bodyEnd - 2); // move view past the header
    
                if (bodyEnd != 0)
                    goto exitFailure;

                m_curChunkSize = -1;
                m_curChunkPos = 0;

            }
            else
            {
                // stream, much like regular body here
                size_t bytesLeft = m_curChunkSize - m_curChunkPos;
                size_t bytesSending = (bytesLeft > remaining.size()) ? remaining.size() : bytesLeft;
            
                BufferView temp = remaining.substr(bytesSending, remaining.size() - bytesSending);
                m_curChunkPos += bytesSending;
            
                if (m_response)
                    m_response->receiveRequestBody(remaining.substr(0, bytesSending));
                
                remaining = temp;
            }
        }
    }

    return (remaining);

exitFailure:

    m_parsingState = ERROR;
    m_data.status = Http::Status::BAD_REQUEST;      // chunk header too long
    
    if (m_response)
        m_response->receiveRequestBody(BufferView()); // send "eof" to signal end of body
    
    m_parsingFunction = &Request::mf_handleNothing;
    
    return (BufferView());
}