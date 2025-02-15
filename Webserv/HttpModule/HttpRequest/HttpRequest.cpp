/* *********************************/
/*                                 */
/*   HttpRequest.cpp               */
/*   - implements the HttpRequest  */
/*    class.                       */
/*                                 */
/* *********************************/

#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
    : m_status(Http::Status::OK)
    , m_timeout(30) // 30 seconds default timeout
    , m_httpConn(NULL)
    , m_parsingState(IDLE)
{}

HttpRequest::~HttpRequest()
{}


int HttpRequest::mf_handleStreamedBody(const std::string& rawData)
{
    m_status = mf_parseBody(rawData);
    if (m_status != Http::Status::OK) {
        m_parsingState = ERROR;
    }

    return m_status;
}

int HttpRequest::mf_handlePostBody(const std::string& rawData, size_t bodyStart)
{
    m_status = mf_parseBody(rawData.substr(bodyStart));
    if (m_status != Http::Status::OK) {
        m_parsingState = ERROR;
        return m_status;
    }

    return m_status;
}

int HttpRequest::mf_parseFirstIncomming(const std::string& rawData)
{
    m_parsingState = STARTED;

    // Parse request line
    size_t pos = rawData.find("\r\n");
    if (pos == std::string::npos) {
        m_status = Http::Status::BAD_REQUEST;
        m_parsingState = ERROR;
        return m_status;
    }

    m_status = mf_parseRequestLine(rawData.substr(0, pos));
    if (m_status != Http::Status::OK) {
        m_parsingState = ERROR;
        return m_status;
    }

    // Parse headers
    size_t headerStart = pos + 2;
    pos = rawData.find("\r\n\r\n", headerStart);
    if (pos == std::string::npos) {
        m_status = Http::Status::BAD_REQUEST;
        m_parsingState = ERROR;
        return m_status;
    }

    m_status = mf_parseHeaders(rawData.substr(headerStart, pos - headerStart));
    if (m_status != Http::Status::OK) {
        m_parsingState = ERROR;
        return m_status;
    }

    // Handle body for POST requests
    if (m_method == "POST") {
        m_status = mf_handlePostBody(rawData, pos + 4);
    }

    m_parsingState = COMPLETED;
    return m_status;
}

int HttpRequest::parse(const std::string& rawData)
{
    try {
        if (m_parsingState == IDLE) {
            m_status = mf_parseFirstIncomming(rawData);
        }

        if (m_parsingState == INCOMPLETE) {
            m_status = mf_handleStreamedBody(rawData);
        }
    }
    catch (const std::exception& e) {
        m_parsingState = ERROR;
        m_status = Http::Status::INTERNAL_ERROR;
    }

    return m_status;
}

// Getters
const HttpRequest::ParsingState& HttpRequest::getParsingState() const
{
    return m_parsingState;
}

const std::string& HttpRequest::getMethod() const
{
    return (m_method);
}

const std::string& HttpRequest::getUri() const
{
    return (m_uri);
}

const std::string& HttpRequest::getHttpVersion() const
{
    return (m_httpVersion);
}

const std::map<std::string, std::string>& HttpRequest::getHeaders() const
{
    return (m_headers);
}

const std::string& HttpRequest::getBody() const
{
    return (m_body);
}

const std::map<std::string, std::string>& HttpRequest::getUriComponents() const
{
    return (m_uriComponents);
}

// Parsing states
bool HttpRequest::isStarted() const
{
    return (m_parsingState == STARTED);
}

bool HttpRequest::isError() const
{
    return (m_parsingState == ERROR);
}

bool HttpRequest::isIncomplete() const
{
    return (m_parsingState == INCOMPLETE);
}

bool HttpRequest::isCompleted() const
{
    return (m_parsingState == COMPLETED);
}
