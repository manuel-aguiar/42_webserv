#include "HttpRequest.hpp"

// Static set initializers
static std::set<std::string> initAllowedMethods()
{
    std::set<std::string> methods;
    methods.insert("GET");
    methods.insert("POST");
    methods.insert("DELETE");
    return methods;
}

// putting all required headers in a set, HTTP 1.1 requires Host in the header
static std::set<std::string> initRequiredHeaders()
{
    std::set<std::string> headers;
    headers.insert("Host");
    return headers;
}

// putting all for now
static std::set<std::string> initOptionalHeaders()
{
    std::set<std::string> headers;
    headers.insert("Accept");
    headers.insert("Accept-Charset");
    headers.insert("Accept-Encoding");
    headers.insert("Accept-Language");
    headers.insert("Authorization");
    headers.insert("Content-Length");
    headers.insert("Content-Type");
    headers.insert("Cookie");
    headers.insert("Date");
    headers.insert("Host");
    headers.insert("If-Match");
    headers.insert("If-Modified-Since");
    headers.insert("If-None-Match");
    headers.insert("If-Range");
    headers.insert("If-Unmodified-Since");
    headers.insert("Max-Forwards");
    headers.insert("Proxy-Authorization");
    headers.insert("Range");
    headers.insert("Referer");
    headers.insert("TE");
    headers.insert("User-Agent");
    return headers;
}

// Initialize static members
const std::set<std::string> HttpRequest::ALLOWED_METHODS = initAllowedMethods();
const std::set<std::string> HttpRequest::REQUIRED_HEADERS = initRequiredHeaders();
const std::set<std::string> HttpRequest::OPTIONAL_HEADERS = initOptionalHeaders();


HttpRequest::HttpRequest()
    : m_status(RequestStatus::OK)
    , m_timeout(30) // 30 seconds default timeout
    , m_httpConn(NULL)
    , m_session(NULL)
{}

HttpRequest::~HttpRequest()
{}

int HttpRequest::parse(const std::string& rawData)
{
    // Reset state before parsing
    m_status = RequestStatus::OK;

    try {
        size_t pos = rawData.find("\r\n"); // goes until the CRLF
        if (pos == std::string::npos)
            return (RequestStatus::BAD_REQUEST);

        // Parse request line
        m_status = mf_parseRequestLine(rawData.substr(0, pos));
        if (m_status != RequestStatus::OK)
            return m_status;

        // TODO: check if logic below breaks
        // Parse headers
        size_t headerStart = pos + 2;
        pos = rawData.find("\r\n\r\n", headerStart);
        if (pos == std::string::npos)
            return (RequestStatus::BAD_REQUEST);

        // Parse headers section
        m_status = mf_parseHeaders(rawData.substr(headerStart, pos - headerStart));
        if (m_status != RequestStatus::OK)
            return m_status;

        // On POST request, parse the body
        if (m_method == "POST") {
            size_t bodyStart = pos + 4;
            m_status = mf_parseBody(rawData.substr(bodyStart));
        }

        return m_status;
    }
    catch (const std::exception& e) {
        return RequestStatus::INTERNAL_ERROR;
    }
}

// Getters
const std::string& HttpRequest::getMethod() const
{
    return m_method;
}

const std::string& HttpRequest::getUri() const
{
    return m_uri;
}

const std::string& HttpRequest::getHttpVersion() const
{
    return m_httpVersion;
}

const std::map<std::string, std::string>& HttpRequest::getHeaders() const
{
    return m_headers;
}

const std::string& HttpRequest::getBody() const
{
    return m_body;
}

const std::map<std::string, std::string>& HttpRequest::getUriComponents() const
{
    return m_uriComponents;
}
