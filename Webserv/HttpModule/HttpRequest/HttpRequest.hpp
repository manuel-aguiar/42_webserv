/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rphuyal <rphuyal@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:17:04 by codespace         #+#    #+#             */
/*   Updated: 2025/01/16 20:46:05 by rphuyal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

// C++ Headers
# include <string>
# include <map>
# include <set>

// Forward declarations
class HttpConnection;
class HttpSession;
class Response;

// Request integrity status codes
struct RequestStatus {
    static const int OK = 200;
    static const int BAD_REQUEST = 400;
    static const int METHOD_NOT_ALLOWED = 405;
    static const int URI_TOO_LONG = 414;
    static const int HEADERS_TOO_LARGE = 431;
    static const int INTERNAL_ERROR = 500;
};


class HttpRequest {
    public:
        HttpRequest();
        ~HttpRequest();

        // Main parsing interface
        int parse(const std::string& rawData);

        // Getters for components
        const std::string& 							getMethod() const;
        const std::string& 							getUri() const;
        const std::string& 							getHttpVersion() const;
        const std::map<std::string, std::string>&	getHeaders() const;
        const std::string& 							getBody() const;

    private:
        // Static validation sets
        static const std::set<std::string> ALLOWED_METHODS;
        static const std::set<std::string> REQUIRED_HEADERS;
        static const std::set<std::string> OPTIONAL_HEADERS;

        static const size_t MAX_URI_LENGTH = 2048;        // Common limit
        static const size_t MAX_HEADER_SIZE = 8192;       // 8KB limit per header
        static const size_t MAX_HEADERS_SIZE = 32768;     // 32KB total headers limit

        // Parsers
        int mf_parseRequestLine(const std::string& line);
        int mf_parseHeaders(const std::string& line);
        int mf_parseBody(const std::string& data);
        int mf_parseChunkedBody(const std::string& data);

        // Validations
        bool mf_validateMethod() const;
        bool mf_validateUri() const;
        bool mf_validateHttpVersion() const;
        bool mf_validateHeaders() const;

        // corresponding response crafter
        Response mf_craftResponse() const;

        // States
        int m_status;
        size_t m_timeout;

        // Connections
        HttpConnection* m_httpConn;
        HttpSession* m_session;

        // Components
        std::string m_method;
        std::string m_uri;
        std::string m_httpVersion;
        std::map<std::string, std::string> m_headers;
        std::map<std::string, std::string> m_uriComponents;
        std::string m_body;

        // Prevent copying
        HttpRequest(const HttpRequest&);
        HttpRequest& operator=(const HttpRequest&);
};

// Static members
const std::set<std::string> HttpRequest::ALLOWED_METHODS = {
    "GET",
    "POST",
    "DELETE"
};

const std::set<std::string> HttpRequest::REQUIRED_HEADERS = {
    "Host"
};

const std::set<std::string> HttpRequest::OPTIONAL_HEADERS = {
    "Accept",
    "Accept-Charset",
    "Accept-Encoding",
    "Accept-Language",
    "Authorization",
    "Content-Length",
    "Content-Type",
    "Cookie",
    "Date",
    "Host",
    "If-Match",
    "If-Modified-Since",
    "If-None-Match",
    "If-Range",
    "If-Unmodified-Since",
    "Max-Forwards",
    "Proxy-Authorization",
    "Range",
    "Referer",
    "TE",
    "User-Agent"
};

#endif