/* *********************************/
/*                                 */
/*   Http::Request.cpp             */
/*   - implements the Http::Request*/
/*    class.                       */
/*                                 */
/* *********************************/

#include "HttpRequest.hpp"

namespace Http
{

///////////// RETAINED FOR TESTING, DELETE WHEN DONE /////////////
Request::Request()
    : m_httpConn(*reinterpret_cast<Http::Connection*>(0))
    , m_serverBlock(NULL)
    , m_serverLocation(NULL)
    , m_status(Http::Status::OK)
    , m_timeout(30) // 30 seconds default timeout
    , m_parsingState(IDLE)
    , m_bodyType(NONE)
    , m_contentType(RAW)
    , m_expectedLength(0)

     {}
/////////////////////////////////////////////////////////////////


Request::Request(Http::Connection& conn)
    : m_httpConn(conn)
    , m_serverBlock(NULL)
    , m_serverLocation(NULL)
    , m_status(Http::Status::OK)
    , m_timeout(30) // 30 seconds default timeout
    , m_parsingState(IDLE)
    , m_bodyType(NONE)
    , m_contentType(RAW)
    , m_expectedLength(0)
{}

Request::~Request()
{}

Request::Request(const Request& copy)
    : m_httpConn(copy.m_httpConn)
    , m_serverBlock(copy.m_serverBlock)
    , m_serverLocation(copy.m_serverLocation)
    , m_status(copy.m_status)
    , m_timeout(copy.m_timeout)
    , m_parsingState(copy.m_parsingState)
    , m_method(copy.m_method)
    , m_uri(copy.m_uri)
    , m_httpVersion(copy.m_httpVersion)
    , m_headers(copy.m_headers)
    , m_path(copy.m_path)
    , m_queryString(copy.m_queryString)
    , m_fragment(copy.m_fragment)
    , m_body(copy.m_body)
    , m_bodyType(copy.m_bodyType)
    , m_contentType(copy.m_contentType)
    , m_expectedLength(copy.m_expectedLength)
    {}

Request&
Request::operator=(const Request& copy)
{
    if (this == &copy) return (*this);

    m_serverBlock = copy.m_serverBlock;
    m_serverLocation = copy.m_serverLocation;
    m_status = copy.m_status;
    m_timeout = copy.m_timeout;
    m_parsingState = copy.m_parsingState;
    m_method = copy.m_method;
    m_uri = copy.m_uri;
    m_httpVersion = copy.m_httpVersion;
    m_headers = copy.m_headers;
    m_path = copy.m_path;
    m_queryString = copy.m_queryString;
    m_fragment = copy.m_fragment;
    m_body = copy.m_body;
    m_bodyType = copy.m_bodyType;
    m_contentType = copy.m_contentType;
    m_expectedLength = copy.m_expectedLength;
    return (*this);
}

void Request::mf_handleRequestLine(const BufferView& buffer)
{
    size_t reqLineEnd = buffer.find("\r\n", 2, 0);
    if (reqLineEnd == std::string::npos) return; // not enough to go through yets

    BufferView requestLine(buffer.substr(0, reqLineEnd));
    m_status = mf_parseRequestLine(requestLine);
    if (m_status != Http::Status::OK) {
        m_parsingState = ERROR;
        return;
    }

    // transition to headers
    m_parsingState = HEADERS;
}

void Request::mf_handleHeaders(const BufferView& buffer)
{
    size_t headerEnd = buffer.find("\r\n\r\n", 4, 0);
    if (headerEnd == BufferView::npos) return; // not enough to go through yet

    size_t headerStart = buffer.find("\r\n", 2, 0);
    if (headerStart == BufferView::npos) return; // not enough to go through yet

    headerStart += 2;

    BufferView headers(buffer.substr(headerStart, headerEnd - headerStart));
    m_status = mf_parseHeaders(headers);
    if (m_status != Http::Status::OK) {
        m_parsingState = ERROR;
        return;
    }

    // transition to body
    m_parsingState = BODY;
}

void Request::mf_handleBody(const BufferView& buffer)
{
    if (m_method != "POST") {
        m_parsingState = COMPLETED; // no body needed for non POST requests
        return;
    }

    size_t bodyStart = buffer.find("\r\n\r\n", 4, 0);
    if (bodyStart == BufferView::npos) return; // not enough to go through yet

    bodyStart += 4;

    BufferView body(buffer.data() + bodyStart, buffer.size() - bodyStart);
    m_status = mf_parseBody(body);
    if (m_status != Http::Status::OK) {
        m_parsingState = ERROR;
        return;
    }

    // transition to completed
    m_parsingState = COMPLETED;
}

void Request::parse(const BaseBuffer& buffer)
{
    BufferView internalBuffer(buffer.data(), buffer.size());

    try {
        switch (m_parsingState) {
            case IDLE:
                m_parsingState = REQLINE;
                // intentional fallthrough

            case REQLINE:
                mf_handleRequestLine(internalBuffer);
                if (m_parsingState == ERROR || m_parsingState == REQLINE) return;
                // intentional fallthrough

            case HEADERS:
                mf_handleHeaders(internalBuffer);
                if (m_parsingState == ERROR || m_parsingState == HEADERS) return;
                // intentional fallthrough

            case BODY:
                mf_handleBody(internalBuffer);
                if (m_parsingState == ERROR || m_parsingState == BODY) return;

            case ERROR:
            case COMPLETED:
                return;
        }
    }
    catch (const std::exception& e) {
        m_parsingState = ERROR;
        m_status = Http::Status::INTERNAL_ERROR;
    }
}


void    Request::reset()
{
    m_method.clear();
    m_uri.clear();
    m_httpVersion.clear();
    m_headers.clear();
    m_path.clear();
    m_queryString.clear();
    m_fragment.clear();
    m_body.clear();
    m_status = Http::Status::OK;
    m_parsingState = IDLE;
}

// Getters
const Request::ParsingState& Request::getParsingState() const
{
    return m_parsingState;
}

const std::string& Request::getMethod() const
{
    return (m_method);
}

const std::string& Request::getUri() const
{
    return (m_uri);
}

const std::string& Request::getHttpVersion() const
{
    return (m_httpVersion);
}

const std::map<std::string, std::string>& Request::getHeaders() const
{
    return (m_headers);
}

const std::string& Request::getBody() const
{
    return (m_body);
}

const std::string& Request::getPath() const
{
    return (m_path);
}

const std::string& Request::getQueryString() const
{
    return (m_queryString);
}

const std::string& Request::getFragment() const
{
    return (m_fragment);
}

Http::Status::Number Request::getStatus() const
{
    return (m_status);
}

// Parsing states
bool Request::isStarted() const
{
    return (m_parsingState != IDLE);
}

bool Request::isIncompleted() const
{
    return (m_parsingState == REQLINE || m_parsingState == HEADERS || m_parsingState == BODY);
}

bool Request::isCompleted() const
{
    return (m_parsingState == COMPLETED);
}

bool Request::isError() const
{
    return (m_parsingState == ERROR);
}

} // namespace Http
