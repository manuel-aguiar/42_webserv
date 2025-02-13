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

// Helper function to convert string to size_t
static size_t strToSizeT(const std::string& str, int base = 10) {
    std::istringstream iss(str);
    size_t value;

    if (base == 16) {
        unsigned long ul;
        iss >> std::hex >> ul;
        if (iss.fail() || !iss.eof())
            throw std::runtime_error("Invalid number format");
        value = static_cast<size_t>(ul);
    } else {
        iss >> value;
        if (iss.fail() || !iss.eof())
            throw std::runtime_error("Invalid number format");
    }
    return value;
}

int HttpRequest::mf_parseBody(const std::string& data)
{
    try {
        // Check if we have a Content-Length header
        if (m_headers.find("Content-Length") == m_headers.end())
            return Http::Status::BAD_REQUEST;

        // Parse and validate Content-Length
        size_t contentLength;
        try {
            contentLength = strToSizeT(m_headers["Content-Length"]);
        } catch (const std::exception& e) {
            return Http::Status::BAD_REQUEST;
        }

        // Check if body size exceeds maximum allowed
        if (contentLength > Http::HttpStandard::MAX_BODY_LENGTH)
            return Http::Status::PAYLOAD_TOO_LARGE;

        // Check if actual data length matches Content-Length
        if (data.length() != contentLength)
            return Http::Status::BAD_REQUEST;

        // Store the body
        m_body = data;
        return Http::Status::OK;
    }
    catch (const std::exception& e) {
        return Http::Status::INTERNAL_ERROR;
    }
}

int HttpRequest::mf_parseChunkedBody(const std::string& data)
{
    try {
        size_t pos = 0;
        std::string assembled_body;

        while (pos < data.length()) {
            // Find chunk size line
            size_t lineEnd = data.find("\r\n", pos);
            if (lineEnd == std::string::npos)
                return Http::Status::BAD_REQUEST;

            // Parse chunk size (in hex)
            std::string chunkSizeStr = data.substr(pos, lineEnd - pos);

            // Remove chunk extensions if present
            size_t semicolonPos = chunkSizeStr.find(';');
            if (semicolonPos != std::string::npos)
                chunkSizeStr = chunkSizeStr.substr(0, semicolonPos);

            // Convert hex string to size_t
            size_t chunkSize;
            try {
                chunkSize = strToSizeT(chunkSizeStr, 16);
            } catch (const std::exception& e) {
                return Http::Status::BAD_REQUEST;
            }

            // Zero-sized chunk indicates end of body
            if (chunkSize == 0) {
                m_body = assembled_body;
                return Http::Status::OK;
            }

            // Validate chunk size
            if (chunkSize > Http::HttpStandard::MAX_CHUNK_SIZE)
                return Http::Status::PAYLOAD_TOO_LARGE;

            // Move position to start of chunk data
            pos = lineEnd + 2;

            // Check if we have enough data for this chunk
            if (pos + chunkSize + 2 > data.length())
                return Http::Status::BAD_REQUEST;

            // Extract chunk data
            assembled_body += data.substr(pos, chunkSize);

            // Move position to next chunk
            pos += chunkSize + 2; // +2 for CRLF after chunk data

            // Check if total body size exceeds maximum
            if (assembled_body.length() > Http::HttpStandard::MAX_BODY_LENGTH)
                return Http::Status::PAYLOAD_TOO_LARGE;
        }

        return Http::Status::BAD_REQUEST; // No zero-sized chunk found
    }
    catch (const std::exception& e) {
        return Http::Status::INTERNAL_ERROR;
    }
}
