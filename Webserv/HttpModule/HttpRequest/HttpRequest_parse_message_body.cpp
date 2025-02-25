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

// Helper function to convert string to integer, takes a base
static int strToInteger(const std::string& str, int base = 10)
{
    std::string trimmed = str;
    trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
    trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);

    if (trimmed.empty()) {
        throw std::runtime_error("Invalid number format: empty string");
    }

    // Use strtol for parsing (C++98 compatible)
    char* endptr;
    long value = strtol(trimmed.c_str(), &endptr, base);

    // Check if conversion was successful
    if (*endptr != '\0') {
        throw std::runtime_error("Invalid number format");
    }

    // Check if value fits in an int
    if (value > INT_MAX || value < INT_MIN) {
        throw std::runtime_error("Number out of int range");
    }

    return static_cast<int>(value);
}

ChunkInfo Http::Request::mf_parseChunkHeader(const std::string& data, size_t pos)
{
    ChunkInfo chunk = {0, 0};
    size_t lineEnd = data.find("\r\n", pos);
    if (lineEnd == std::string::npos || lineEnd == pos) {
        return chunk;
    }

    // Extract and clean chunk size string
    std::string chunkSizeStr = data.substr(pos, lineEnd - pos);
    size_t semicolonPos = chunkSizeStr.find(';');
    if (semicolonPos != std::string::npos) {
        chunkSizeStr = chunkSizeStr.substr(0, semicolonPos);
    }

    try {
        // Parse chunk size as hex
        size_t chunkSize = static_cast<size_t>(strToInteger(chunkSizeStr, 16));

        // Validate chunk size
        if (chunkSize > Http::HttpStandard::MAX_CHUNK_SIZE) {
            chunk.size = 0;
            chunk.headerEnd = 0;
            return chunk;
        }

        chunk.size = chunkSize;
        chunk.headerEnd = lineEnd + 2;  // CRLF
    }
    catch (const std::exception& e) {
        // Invalid chunk size format
        chunk.size = 0;
        chunk.headerEnd = 0;
    }

    return chunk;
}

bool Http::Request::mf_validateAndExtractChunk(const std::string& data, const ChunkInfo& chunk, size_t& pos, std::string& assembled_body)
{
    // Check if we have enough data for this chunk
    if (chunk.headerEnd + chunk.size + 2 > data.length()) {
        return false;
    }

    // Verify chunk ends with \r\n
    if (data.substr(chunk.headerEnd + chunk.size, 2) != "\r\n") {
        m_parsingState = ERROR;
        return false;
    }

    // Extract chunk data
    assembled_body += data.substr(chunk.headerEnd, chunk.size);

    // Move position to next chunk
    pos = chunk.headerEnd + chunk.size + 2;
    return true;
}

// transfer-encoding: chunked
// last raw body = "5\r\nHello\r\n" -> total: 10 bytes
// m_data.body = "Hello"
// status = BODY
// next raw body = "5\r\nWorld\r\n" -> total: 10 bytes
// m_body = "Hello World"
// status = BODY
// next raw body = "3\r\n!!!\r\n" -> total: 10 bytes
// m_body = "Hello World!!!"
// status = COMPLETED

Http::Status::Number
Http::Request::mf_parseChunkedBody(const std::string& data)
{
    try {
        std::string assembled_body;
        size_t pos = 0;

        while (pos < data.length()) {
            // Parse chunk header
            ChunkInfo chunk = mf_parseChunkHeader(data, pos);
            if (chunk.headerEnd == 0) {
                m_parsingState = ERROR;
                return Http::Status::BAD_REQUEST;
            }

            // Check for final chunk
            if (chunk.size == 0) {
                size_t endPos = data.find("\r\n\r\n", chunk.headerEnd);
                if (endPos != std::string::npos) {
                    m_data.body = assembled_body;
                    m_parsingState = COMPLETED;
                    return Http::Status::OK;
                }

                m_parsingState = BODY;
                return Http::Status::OK;
            }

            // Check if chunk size exceeds limit
            if (chunk.size > Http::HttpStandard::MAX_CHUNK_SIZE) {
                m_parsingState = ERROR;
                return Http::Status::PAYLOAD_TOO_LARGE;
            }

            // Ensure we have the full chunk data
            if (chunk.headerEnd + chunk.size + 2 > data.length()) {
                m_data.body = assembled_body;
                m_parsingState = BODY;
                return Http::Status::OK;
            }

            // Validate chunk format (must end with CRLF)
            if (data.substr(chunk.headerEnd + chunk.size, 2) != "\r\n") {
                m_parsingState = ERROR;
                return Http::Status::BAD_REQUEST;
            }

            assembled_body += data.substr(chunk.headerEnd, chunk.size);
            pos = chunk.headerEnd + chunk.size + 2;

            // Look for end chunk after processing current chunk
            size_t endChunk = data.find("\r\n\r\n", pos);
            if (endChunk != std::string::npos) {
                m_data.body = assembled_body;
                m_parsingState = COMPLETED;
                return Http::Status::OK;
            }
        }

        // Update body and state
        m_data.body = assembled_body;
        ASSERT_EQUAL(m_response != NULL, true, "Request::mf_parseChunkedBody(), m_response is NULL");

        // before state transition, call response to check
        m_response->receiveRequestBody(m_data.body);
        m_data.expectedLength = m_data.body.length();
        m_parsingState = BODY;
        return Http::Status::OK;
    }
    catch (const std::exception& e) {
        m_parsingState = ERROR;
        return Http::Status::BAD_REQUEST;
    }
}


// content-length: 10
// last raw body = "Hello" -> total: 5 bytes
// m_data.body = "Hello"
// status = BODY
// next raw body = "World" -> total: 5 bytes
// m_data.body = "Hello World"
// status = COMPLETED
// next raw body = "!!!" -> total: 3 bytes
// #ignored, we already have the full body for this request

BufferView Http::Request::mf_parseChunkedBody(BaseBuffer& buffer, const BufferView& currentView)
{
    (void)buffer;
    (void)currentView;

    return (BufferView());
}

BufferView Http::Request::mf_parseRegularBody(BaseBuffer& buffer, const BufferView& currentView)
{
    size_t bytesLeft = m_curContentLength - m_curContentPos;
    size_t bytesSending = (bytesLeft > currentView.size()) ? currentView.size() : bytesLeft;

    BufferView remaining = currentView.substr(bytesSending, currentView.size() - bytesSending);
    m_curContentPos += bytesSending;

    //std::cout << currentView << std::endl;

    if (m_response)
        m_response->receiveRequestBody(currentView.substr(0, bytesSending));

    buffer.truncatePush(remaining);

    if (m_curContentPos == m_curContentLength)
    {
        
        m_parsingState = COMPLETED;
        if (m_response)
            m_response->receiveRequestBody(BufferView()); // send empty body, signals end of body
    }

    return (BufferView());
}

Http::Status::Number
Http::Request::mf_parseRegularBody(const std::string& data)
{
    int contentLengthInt;
    try {
        contentLengthInt = strToInteger(m_data.headers["Content-Length"]);
    } catch (const std::exception& e) {
        return Http::Status::BAD_REQUEST; // since the value was not a valid number
    }

    // negative value are not accepted
    if (contentLengthInt < 0) {
        m_parsingState = ERROR;
        return Http::Status::BAD_REQUEST;
    }

    size_t contentLength = static_cast<size_t>(contentLengthInt);
    m_data.expectedLength = contentLength;
    // TODO: later get this from server config
    if (contentLength > Http::HttpStandard::MAX_BODY_LENGTH) {
        return Http::Status::PAYLOAD_TOO_LARGE;
    }

    // append to existing body
    m_data.body += data;

    ASSERT_EQUAL(m_response != NULL, true, "Request::mf_parseRegularBody(), m_response is NULL");

    // before state transition, call response to check
    m_response->receiveRequestBody(BufferView(m_data.body));

    if (m_data.body.length() == contentLength) {
        m_parsingState = COMPLETED;
        return Http::Status::OK;
    } else if (m_data.body.length() > contentLength) {
        m_parsingState = ERROR;
        return Http::Status::BAD_REQUEST;
    }

    // we don't have the full body yet
    m_parsingState = BODY;
    return Http::Status::OK;
}

Http::Status::Number
Http::Request::mf_parseMultipartData(const std::string& data)
{
    static_cast<void>(data);
    return Http::Status::OK;
}

Http::RequestData::BodyType
Http::Request::mf_bodyType()
{
    if (m_data.headers.find("Transfer-Encoding") != m_data.headers.end() &&
        m_data.headers["Transfer-Encoding"] == "chunked") {
        return Http::RequestData::CHUNKED;
    }

    return Http::RequestData::REGULAR;
}

Http::RequestData::ContentType
Http::Request::mf_contentType()
{
    if (m_data.headers.find("Content-Type") != m_data.headers.end() &&
        m_data.headers["Content-Type"] == "multipart/form-data") {
        return Http::RequestData::MULTIPART;
    }

    return Http::RequestData::RAW;
}

Http::Status::Number
Http::Request::mf_parseBody(const BufferView& receivedView)
{
    // TODO: do a better handling of the body with a receivedView view
    std::string data;
    receivedView.to_string(data);

    try {
        if (m_data.bodyType == Http::RequestData::NONE) {
            m_data.bodyType = mf_bodyType();
            m_data.contentType = mf_contentType();
        }

        // Get raw bytes according to transfer type
        std::string extractedData;
        if (m_data.bodyType == Http::RequestData::CHUNKED) {
            m_data.status = mf_parseChunkedBody(data);
        } else {
            m_data.status = mf_parseRegularBody(data);
        }

        if (m_data.status != Http::Status::OK) {
            m_parsingState = ERROR;
            return m_data.status;
        }

        // Interpreter for the received body
        if (m_data.contentType == Http::RequestData::MULTIPART) {
            m_data.status = mf_parseMultipartData(extractedData);
        } else {
            m_data.body += extractedData;
            if (m_data.body.length() == m_data.expectedLength) {
                m_parsingState = COMPLETED;
            }
        }
    }
    catch (const std::exception& e) {
        m_parsingState = ERROR;
        m_data.status = Http::Status::INTERNAL_ERROR;
    }

    return m_data.status;
}
*/