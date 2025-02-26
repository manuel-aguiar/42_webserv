
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

/*
	@returns: BufferView of the remaining data that wasn't consumed
*/
BufferView Http::Request::mf_parseChunkedBody_GetChunk(const BufferView& receivedView)
{
    //std::cout << "entered get chunk" << std::endl;
    const BufferView delimiter("\r\n", 2);
    BufferView remaining = receivedView;
    BufferView thisChunkSize;
    
    if (remaining.size() <= delimiter.size())
        return (remaining); // not enough to go through yet
    
    size_t headerEnd = remaining.find(delimiter, m_findPivot);
    if (headerEnd == BufferView::npos)
    {
        m_findPivot = std::max((int)remaining.size() - (int)delimiter.size(), 0);

        // HARD LIMIT, single header size cannot be bigger than the buffer capacity
        if (remaining.size() >= m_bufferCapacity)
            return (mf_parseBodyExitError(Http::Status::BAD_REQUEST)); // chunk header too long
        return (remaining); // not enough to go through yet
    }
    
    m_findPivot = 0;

    thisChunkSize = remaining.substr(0, headerEnd);
    remaining = remaining.substr(headerEnd + delimiter.size(), remaining.size() - headerEnd - delimiter.size()); // move view past the header

    m_curChunkSize = strToInteger(thisChunkSize, 16);
    if (m_curChunkSize == -1)
    return (mf_parseBodyExitError(Http::Status::BAD_REQUEST)); 

    m_curChunkPos = 0;

    m_parsingFunction = &Request::mf_parseChunkedBody_ParseChunk;
    if (m_curChunkSize == 0)
        m_parsingFunction = &Request::mf_parseChunkedBody_EndChunk;

    return ((this->*m_parsingFunction)(remaining));
}

/*
	@returns: BufferView of the remaining data that wasn't consumed
*/
BufferView Http::Request::mf_parseChunkedBody_ParseChunk(const BufferView& receivedView)
{
    //std::cout << "entered parse chunk" << std::endl;
    BufferView remaining = receivedView;

    if (remaining.size() == 0)
        return (remaining); // not enough to go through yet

    // stream, much like regular body here
    size_t      bytesLeft = m_curChunkSize - m_curChunkPos;
    size_t      bytesSending = (bytesLeft > remaining.size()) ? remaining.size() : bytesLeft;
    BufferView  unconsumed;

    if (m_response && bytesSending > 0)
        unconsumed = m_response->receiveRequestBody(remaining.substr(0, bytesSending));

    size_t bytesConsumed = bytesSending - unconsumed.size();

    BufferView temp = remaining.substr(bytesConsumed, remaining.size() - bytesConsumed);

    m_curChunkPos += bytesConsumed;

    remaining = temp;

    // if consumed all bytes until border
    if (m_curChunkPos == m_curChunkSize)
        m_parsingFunction = &Request::mf_parseChunkedBody_EndChunk;

    return ((this->*m_parsingFunction)(remaining));

}

/*
	@returns: BufferView of the remaining data that wasn't consumed
*/
BufferView Http::Request::mf_parseChunkedBody_EndChunk(const BufferView& receivedView)
{
    //std::cout << "entered end chunk" << std::endl;
    const BufferView delimiter("\r\n", 2);
    BufferView remaining = receivedView;

    if (remaining.size() <= delimiter.size())
        return (remaining); // not enough to go through yet

    size_t bodyEnd = remaining.find(delimiter, m_findPivot);
    // not found, last character in view or not followed by \n
    if (bodyEnd == BufferView::npos)
    {
        m_findPivot = std::max((int)remaining.size() - (int)delimiter.size(), 0);
        // HARD LIMIT, single header size cannot be bigger than the buffer capacity
        if (remaining.size() >= m_bufferCapacity)
            return (mf_parseBodyExitError(Http::Status::BAD_REQUEST)); 
        return (remaining); // not enough to go through yet
    }
    
    m_findPivot = 0;
    
    remaining = remaining.substr(bodyEnd + delimiter.size(), remaining.size() - bodyEnd - delimiter.size()); // move view past the header

    if (bodyEnd != 0)
    return (mf_parseBodyExitError(Http::Status::BAD_REQUEST)); 

    if (m_curChunkSize == 0)
    {
        m_parsingState = COMPLETED;
        if (m_response)
            m_response->receiveRequestBody(BufferView()); // send "eof" to signal end of body
        m_parsingFunction = &Request::mf_handleNothing;
    }
    else
    {
        m_curChunkSize = -1;
        m_curChunkPos = 0;
        m_parsingFunction = &Request::mf_parseChunkedBody_GetChunk;
    }

    return (this->*m_parsingFunction)(remaining);
}
