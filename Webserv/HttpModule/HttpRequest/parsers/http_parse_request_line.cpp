/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parse_request_line.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rphuyal <rphuyal@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 21:15:00 by rphuyal           #+#    #+#             */
/*   Updated: 2025/01/16 22:26:46 by rphuyal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

std::string httpURIDecoder(const std::string& encoded)
{
    std::string decodedString;

    for (size_t i = 0; i < encoded.length(); ++i) {
        if (encoded[i] == '%') {
            if (i + 2 >= encoded.length())
                throw HttpRequest::EncodingException("Invalid URL encoding");

            int hi = hexToInt(encoded[i + 1]);
            int lo = hexToInt(encoded[i + 2]);
            if (hi == -1 || lo == -1)
                throw HttpRequest::EncodingException("Invalid URL encoding");

            decodedString += static_cast<char>((hi << 4) | lo);
            i += 2;
        }
        else if (encoded[i] == '+') {
            decodedString += ' ';
        }
        else {
            decodedString += encoded[i];
        }
    }
    return decodedString;
}

// URI parsing utilities
void parseUri(const std::string& uri, std::map<std::string, std::string>& components)
{
    size_t query_start = uri.find('?');

    // path can be encoded
    components["path"] = httpURIDecoder(uri.substr(0, query_start));

    if (query_start == std::string::npos)
        return;

    std::string query = uri.substr(query_start + 1);

    size_t start = 0;
    while (start < query.length()) {
        size_t equals = query.find('=', start);
        size_t amp = query.find('&', start);

        if (equals == std::string::npos)
            break;

        // query keys can be encoded
        std::string key = httpURIDecoder(query.substr(start, equals - start));

        // query values can also be encoded
        size_t value_length = amp == std::string::npos ? std::string::npos : amp - equals - 1;
        std::string value = httpURIDecoder(query.substr(equals + 1, value_length));
        components[key] = value;

        if (amp == std::string::npos)
            break;

        start = amp + 1;
    }

    // get rid of the fragment, if any. server does not need it
    size_t fragment_start = uri.find('#');
    if (fragment_start != std::string::npos)
        components["path"] = components["path"].substr(0, fragment_start);
}

int HttpRequest::mf_parseRequestLine(const std::string& line)
{
    size_t first_space = line.find(' ');
    size_t last_space = line.rfind(' ');

    // this check is important because it ensures that the request line is valid
    // and that it has at least three components
    if (first_space == std::string::npos || \
        last_space == std::string::npos || \
        first_space == last_space)
        return RequestStatus::BAD_REQUEST;

    // components, delimited by spaces (As per HTTP/1.1 specification)
    m_method = line.substr(0, first_space);
    m_uri = line.substr(first_space + 1, last_space - first_space - 1);
    m_httpVersion = line.substr(last_space + 1);

    // accepting only valid components
    if (ALLOWED_METHODS.find(m_method) == ALLOWED_METHODS.end())
        return RequestStatus::METHOD_NOT_ALLOWED;

    if (m_uri.length() > MAX_URI_LENGTH)
        return RequestStatus::URI_TOO_LONG;

    if (m_httpVersion != "HTTP/1.1")
        return RequestStatus::BAD_REQUEST;

    try {
        // decode the URI
        parseUri(m_uri, m_uriComponents);
    }
    catch (const HttpRequest::EncodingException& e) {
        return RequestStatus::BAD_REQUEST;
    }

    return RequestStatus::OK;
}