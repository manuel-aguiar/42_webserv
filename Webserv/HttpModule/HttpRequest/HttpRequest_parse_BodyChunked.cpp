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
        if (remaining.size() >= m_readBuffer->capacity())
            return (mf_bodyExitError(remaining, Http::Status::PAYLOAD_TOO_LARGE));
        return (remaining); // not enough to go through yet
    }

    m_findPivot = 0;

    thisChunkSize = remaining.substr(0, headerEnd);
    remaining = remaining.substr(headerEnd + delimiter.size(), remaining.size() - headerEnd - delimiter.size()); // move view past the header

    m_curChunkSize = strToInteger(thisChunkSize, 16);
    if (m_curChunkSize == -1)
        return (mf_bodyExitError(remaining, Http::Status::BAD_REQUEST));

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
    BufferView remaining = receivedView;

    if (remaining.size() == 0)
        return (remaining); // not enough to go through yet

    // stream, much like regular body here
    size_t      bytesLeft = m_curChunkSize - m_curChunkPos;
    size_t      bytesSending = (bytesLeft > remaining.size()) ? remaining.size() : bytesLeft;
    BufferView  unconsumed;

    if (m_httpResponse && bytesSending > 0)
        unconsumed = m_httpResponse->receiveRequestBody(remaining.substr(0, bytesSending));

    size_t bytesConsumed = bytesSending - unconsumed.size();

    BufferView temp = remaining.substr(bytesConsumed, remaining.size() - bytesConsumed);

    m_curChunkPos += bytesConsumed;

    m_totalReadCounter += bytesConsumed;
    if (m_totalReadCounter > m_maxBodySize)
        return (mf_bodyExitError(temp, Http::Status::PAYLOAD_TOO_LARGE));

    remaining = temp;

    // if consumed all bytes until border
    if (m_curChunkPos == m_curChunkSize)
    {
        m_parsingFunction = &Request::mf_parseChunkedBody_EndChunk;
        return (Request::mf_parseChunkedBody_EndChunk(remaining));
    }
    return (remaining);
}

/*
	@returns: BufferView of the remaining data that wasn't consumed
*/
BufferView Http::Request::mf_parseChunkedBody_EndChunk(const BufferView& receivedView)
{
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
        if (remaining.size() >= m_readBuffer->capacity())
            return (mf_bodyExitError(remaining, Http::Status::PAYLOAD_TOO_LARGE)); // chunk too large, use 413 Payload Too Large
        return (remaining); // not enough to go through yet
    }

    m_findPivot = 0;

    remaining = remaining.substr(bodyEnd + delimiter.size(), remaining.size() - bodyEnd - delimiter.size()); // move view past the header

    if (bodyEnd != 0)
        return (mf_bodyExitError(remaining, Http::Status::BAD_REQUEST));  // 400 - Malformed chunk delimiter

    if (m_curChunkSize == 0)
    {
        m_parsingState = COMPLETED;
        if (m_httpResponse)
            m_httpResponse->receiveRequestBody(BufferView()); // send "eof" to signal end of body
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
