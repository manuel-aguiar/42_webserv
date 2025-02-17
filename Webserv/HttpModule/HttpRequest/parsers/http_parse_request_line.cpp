/* *********************************/
/*                                 */
/*   http_parse_request_line.cpp   */
/*   - the request line parser for */
/*    an incoming http request.    */
/*                                 */
/* *********************************/

#include "../HttpRequest.hpp"


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
std::string HttpRequest::mf_decodeUri(const std::string& encoded, bool strict) const
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

        if (strict && encoded[i] == '+') {
            decodedString += ' ';
            continue;
        }

        decodedString += encoded[i];
    }
    return decodedString;
}

// URI parsing utilities
int HttpRequest::mf_parseUriComponents(const std::string& uri)
{
    size_t pathLength = uri.find('?');

    // Parse path component
    m_uriComponents["path"] = mf_decodeUri(uri.substr(0, pathLength), false);

    if (pathLength == std::string::npos)
        return Http::Status::OK;

    size_t equalPos;
    size_t ampPos;
    size_t fragPos;
    size_t valueLength;
    std::string key;
    std::string value;

    size_t start = 0;
    std::string query = uri.substr(pathLength + 1);

    while (start < query.length()) {
        equalPos = query.find('=', start);
        ampPos = query.find('&', start);
        fragPos = query.find('#', start);

        // query keys can be encoded
        key = mf_decodeUri(query.substr(start, equalPos - start));
        valueLength = (ampPos < fragPos ? ampPos : fragPos) - equalPos - 1; // until & or #
        value = mf_decodeUri(query.substr(equalPos + 1, valueLength));

        m_uriComponents[key] = value;

        if (ampPos == std::string::npos)
            break;

        start = ampPos + 1;
    }

    // get rid of the fragment, if any. server does not need it
    size_t fragment_start = uri.find('#');
    if (fragment_start != std::string::npos)
        m_uriComponents["path"] = m_uriComponents["path"].substr(0, fragment_start);

    return Http::Status::OK;
}

int HttpRequest::mf_parseRequestLine(const std::string& line)
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
        m_uri = mf_decodeUri(line.substr(firstSpace + 1, lastSpace - firstSpace - 1), false);
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