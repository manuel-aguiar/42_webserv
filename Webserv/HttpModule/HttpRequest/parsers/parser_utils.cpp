/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rphuyal <rphuyal@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 21:15:00 by rphuyal           #+#    #+#             */
/*   Updated: 2025/01/16 21:32:26 by rphuyal          ###   ########.fr       */
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

std::string urlDecode(const std::string& encoded)
{
    std::string decoded;
    for (size_t i = 0; i < encoded.length(); ++i) {
        if (encoded[i] == '%') {
            if (i + 2 >= encoded.length())
                throw std::runtime_error("Invalid URL encoding");

            int hi = hexToInt(encoded[i + 1]);
            int lo = hexToInt(encoded[i + 2]);
            if (hi == -1 || lo == -1)
                throw std::runtime_error("Invalid URL encoding");

            decoded += static_cast<char>((hi << 4) | lo);
            i += 2;
        }
        else if (encoded[i] == '+') {
            decoded += ' ';
        }
        else {
            decoded += encoded[i];
        }
    }
    return decoded;
}

// URI parsing utilities
void parseUri(const std::string& uri, std::map<std::string, std::string>& components)
{
    // Split path and query
    size_t query_start = uri.find('?');

    // Store the path
    components["path"] = urlDecode(uri.substr(0, query_start));

    // Parse query parameters if present
    if (query_start != std::string::npos) {
        std::string query = uri.substr(query_start + 1);
        size_t start = 0;

        while (start < query.length()) {
            size_t equals = query.find('=', start);
            size_t amp = query.find('&', start);

            if (equals == std::string::npos)
                break;

            std::string key = urlDecode(query.substr(start, equals - start));
            std::string value = urlDecode(query.substr(equals + 1,
                amp == std::string::npos ? std::string::npos : amp - equals - 1));

            components[key] = value;

            if (amp == std::string::npos)
                break;
            start = amp + 1;
        }
    }
}