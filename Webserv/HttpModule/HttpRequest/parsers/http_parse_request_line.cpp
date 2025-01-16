/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parse_request_line.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rphuyal <rphuyal@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 21:15:00 by rphuyal           #+#    #+#             */
/*   Updated: 2025/01/18 13:10:49 by rphuyal          ###   ########.fr       */
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

// not confident
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
    size_t pathLength = uri.find('?');

    // path can be encoded
    components["path"] = httpURIDecoder(uri.substr(0, pathLength));

    if (pathLength == std::string::npos)
        return;

    size_t equalPos;
    size_t ampPos;
    size_t fragPos;
    size_t valueLength;
    size_t start = 0;
    std::string query = uri.substr(pathLength + 1);

    while (start < query.length()) {
        equalPos = query.find('=', start);
        ampPos = query.find('&', start);
        fragPos = query.find('#', start);

        if (equalPos == std::string::npos)
            break;

        // query keys can be encoded
        std::string key = httpURIDecoder(query.substr(start, equalPos - start));

        // value until the next delimiter, & or #
        valueLength = (ampPos < fragPos ? ampPos : fragPos) - equalPos - 1;
        std::string value = httpURIDecoder(query.substr(equalPos + 1, valueLength));
        components[key] = value;

        // if the next delimiter is #, break, we don't care about the fragments
        if (fragPos < ampPos)
            break;

        if (ampPos == std::string::npos || fragPos == std::string::npos)
            break;

        start = ampPos + 1;
    }

    // get rid of the fragment, if any. server does not need it
    size_t fragment_start = uri.find('#');
    if (fragment_start != std::string::npos)
        components["path"] = components["path"].substr(0, fragment_start);
}

int HttpRequest::mf_parseRequestLine(const std::string& line)
{
    size_t firstSpace = line.find(' ');
    size_t lastSpace = line.rfind(' ');

    // this check is important because it ensures that the request line is valid
    // and that it has at least three components
    if (firstSpace == std::string::npos || \
        lastSpace == std::string::npos || \
        firstSpace == lastSpace)
        return RequestStatus::BAD_REQUEST;

    // also only one space between components
    if (line[firstSpace + 1] == ' ' || line[lastSpace - 1] == ' ')
        return RequestStatus::BAD_REQUEST;

    // components, delimited by spaces (As per HTTP/1.1 specification)
    m_method = line.substr(0, firstSpace);
    m_uri = line.substr(firstSpace + 1, lastSpace - firstSpace - 1);
    m_httpVersion = line.substr(lastSpace + 1);

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