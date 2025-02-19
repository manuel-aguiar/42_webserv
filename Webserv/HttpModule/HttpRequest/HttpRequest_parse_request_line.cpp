/* *********************************/
/*                                 */
/*   http_parse_request_line.cpp   */
/*   - the request line parser for */
/*    an incoming http request.    */
/*                                 */
/* *********************************/

// Project headers
#include "HttpRequest.hpp"


// URL decoding utilities
static int hexToInt(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

// not confident
std::string Http::Request::mf_decodeUriComp(const std::string& encoded) const
{
    std::string decodedString;

    for (size_t i = 0; i < encoded.length(); ++i) {
        if (encoded[i] == '%') {
            if (i + 2 >= encoded.length())
                throw EncodingException("Invalid URL encoding");

            // not confident
            int hi = hexToInt(encoded[i + 1]);
            int lo = hexToInt(encoded[i + 2]);
            if (hi == -1 || lo == -1)
                throw EncodingException("Invalid URL encoding");

            decodedString += static_cast<char>((hi << 4) | lo);
            i += 2;
            continue;
        }

        decodedString += encoded[i];
    }
    return decodedString;
}

// URI parsing utilities
Http::Status::Number
Http::Request::mf_parseUriComponents(const std::string& uri)
{
    size_t queryStart = uri.find('?');
    size_t fragmentStart = uri.find('#');

    // get path
    size_t pathStart = queryStart != std::string::npos ? queryStart : fragmentStart;
    m_path = mf_decodeUriComp(uri.substr(0, pathStart));

    // get query string
    if (queryStart != std::string::npos) {
        size_t queryEnd = (fragmentStart != std::string::npos) ? fragmentStart : uri.length();
        m_queryString = uri.substr(queryStart + 1, queryEnd - (queryStart + 1));
    }

    // get fragment
    if (fragmentStart != std::string::npos) {
        m_fragment = uri.substr(fragmentStart + 1);
    }

    return Http::Status::OK;
}

Http::Status::Number
Http::Request::mf_parseRequestLine(const std::string& line)
{
    try {
        size_t firstSpace = line.find(' ');
        size_t lastSpace = line.rfind(' ');

        // this check is important because it ensures that the request line is valid
        // and that it has at least three components
        if (firstSpace == std::string::npos || \
            lastSpace == std::string::npos || \
            firstSpace == lastSpace)
            return Http::Status::BAD_REQUEST;

        // also only one space between components
        if (line[firstSpace + 1] == ' ' || line[lastSpace - 1] == ' ')
            return Http::Status::BAD_REQUEST;

        m_method = line.substr(0, firstSpace);
        m_uri = line.substr(firstSpace + 1, lastSpace - firstSpace - 1);
        m_httpVersion = line.substr(lastSpace + 1);

        // Check if method is allowed using HttpDefinitions
        // TODO: later get this from server config
        const std::set<std::string>& allowedMethods = Http::AllowedRequestMethods::getAllowedMethods();
        if (allowedMethods.find(m_method) == allowedMethods.end())
            return Http::Status::METHOD_NOT_ALLOWED;

        // TODO: later get this from server config
        if (m_uri.length() > Http::HttpStandard::MAX_URI_LENGTH)
            return Http::Status::URI_TOO_LONG;

        if (m_httpVersion != Http::HttpStandard::HTTP_VERSION)
            return Http::Status::BAD_REQUEST;

        return mf_parseUriComponents(m_uri);
    }
    catch (const EncodingException& e) {
        return Http::Status::BAD_REQUEST;
    }
    catch (const std::exception& e) {
        return Http::Status::INTERNAL_ERROR;
    }
}