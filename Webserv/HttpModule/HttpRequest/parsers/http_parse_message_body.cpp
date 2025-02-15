/* *********************************/
/*                                 */
/*   http_parse_message_body.cpp   */
/*   - the message body parser for */
/*    an incoming http request.    */
/*                                 */
/* *********************************/

#include "../HttpRequest.hpp"
#include <sstream>
#include <cstdlib>

// Helper function to convert string to size_t, takes a base
static size_t strToSizeT(const std::string& str, int base = 10)
{
    std::istringstream iss(str);
    size_t value;

    if (base == 16) {
        unsigned long ul;
        iss >> std::hex >> ul;
        value = static_cast<size_t>(ul);
    } else {
        iss >> value;
    }

    if (iss.fail() || !iss.eof()) {
        throw std::runtime_error("Invalid number format");
    }
    return value;
}

ChunkInfo HttpRequest::mf_parseChunkHeader(const std::string& data, size_t pos)
{
    ChunkInfo chunk = {0, 0};
    size_t lineEnd = data.find("\r\n", pos);
    if (lineEnd == std::string::npos) {
        return chunk;
    }

    // Extract and clean chunk size string
    std::string chunkSizeStr = data.substr(pos, lineEnd - pos);
    size_t semicolonPos = chunkSizeStr.find(';');
    if (semicolonPos != std::string::npos) {
        chunkSizeStr = chunkSizeStr.substr(0, semicolonPos);
    }

    // Parse chunk size
    size_t chunkSize = strToSizeT(chunkSizeStr, 16);

    // Validate chunk size
    if (chunkSize > Http::HttpStandard::MAX_CHUNK_SIZE) {
        return chunk;
    }

    chunk.size = chunkSize;
    chunk.headerEnd = lineEnd + 2;
    return chunk;
}

bool HttpRequest::mf_validateAndExtractChunk(const std::string& data, const ChunkInfo& chunk, size_t& pos,std::string& assembled_body)
{
    // Check if we have enough data for this chunk
    if (chunk.headerEnd + chunk.size + 2 > data.length()) {
        m_parsingState = INCOMPLETE;
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
// m_body = "Hello"
// status = INCOMPLETE
// next raw body = "5\r\nWorld\r\n" -> total: 10 bytes
// m_body = "Hello World"
// status = INCOMPLETE
// next raw body = "3\r\n!!!\r\n" -> total: 10 bytes
// m_body = "Hello World!!!"
// status = COMPLETED
int HttpRequest::mf_parseChunkedBody(const std::string& data)
{
    if (data.empty() && m_parsingState == INCOMPLETE) {
        m_parsingState = ERROR;
        return Http::Status::BAD_REQUEST;
    }

    try {
        std::string _body;
        size_t pos = 0;

        while (pos < data.length()) {
            ChunkInfo chunk = mf_parseChunkHeader(data, pos);

            // Zero-sized chunk indicates end of body
            if (chunk.size == 0) {
                m_body = _body;
                return Http::Status::OK;
            }

            // Validate and extract chunk data
            if (!mf_validateAndExtractChunk(data, chunk, pos, _body)) {
                m_parsingState = ERROR;
                return Http::Status::BAD_REQUEST;
            }

            // Check total body size
            if (_body.length() > Http::HttpStandard::MAX_BODY_LENGTH) {
                m_parsingState = ERROR;
                return Http::Status::PAYLOAD_TOO_LARGE;
            }
        }

        if (data.find("0\r\n\r\n") == std::string::npos)
            m_parsingState = INCOMPLETE;
        else
            m_parsingState = COMPLETED;

        return Http::Status::OK; // No zero-sized chunk found
    }
    catch (const std::exception& e) {
        m_parsingState = ERROR;
        return Http::Status::INTERNAL_ERROR;
    }
}


// content-length: 10
// last raw body = "Hello" -> total: 5 bytes
// m_body = "Hello"
// status = INCOMPLETE
// next raw body = "World" -> total: 5 bytes
// m_body = "Hello World"
// status = COMPLETED
// next raw body = "!!!" -> total: 3 bytes
// #ignored, we already have the full body for this request
int HttpRequest::mf_parsePlainBody(const std::string& data)
{
    size_t contentLength;
    try {
        contentLength = strToSizeT(m_headers["Content-Length"]);
    } catch (const std::exception& e) {
        return Http::Status::BAD_REQUEST; // since the value was not a valid number
    }

    // TODO: later get this from server config
    if (contentLength > Http::HttpStandard::MAX_BODY_LENGTH) {
        return Http::Status::PAYLOAD_TOO_LARGE;
    }

    // append to existing body
    m_body += data;

    if (m_body.length() == contentLength) {
        m_parsingState = COMPLETED;
        return Http::Status::OK;
    } else if (m_body.length() > contentLength) {
        m_parsingState = ERROR;
        return Http::Status::BAD_REQUEST;
    }

    // we don't have the full body yet
    m_parsingState = INCOMPLETE;
    return Http::Status::OK;
}

int HttpRequest::mf_parseBody(const std::string& data)
{
    try {
        // check which type of body we are parsing
        if (m_headers.find("Transfer-Encoding") != m_headers.end() &&
            m_headers["Transfer-Encoding"] == "chunked") {
            m_status = mf_parseChunkedBody(data);
        }

        else if (m_headers.find("Content-Length") != m_headers.end()) {
            m_status = mf_parsePlainBody(data);
        }

        else {
            // no header match means something is wrong
            m_parsingState = ERROR;
            m_status = Http::Status::BAD_REQUEST;
        }
    }
    catch (const std::exception& e) {
        m_parsingState = ERROR;
        m_status = Http::Status::INTERNAL_ERROR;
    }

    return m_status;
}
