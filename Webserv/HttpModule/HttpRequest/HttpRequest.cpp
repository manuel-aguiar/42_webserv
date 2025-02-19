/* *********************************/
/*                                 */
/*   Http::Request.cpp             */
/*   - implements the Http::Request*/
/*    class.                       */
/*                                 */
/* *********************************/

#include "HttpRequest.hpp"
#include "../../GenericUtils/Buffer/Buffer.hpp"
#include "../../GenericUtils/BufferView/BufferView.hpp"

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

     {}
/////////////////////////////////////////////////////////////////


Request::Request(Http::Connection& conn)
    : m_httpConn(conn)
    , m_serverBlock(NULL)
    , m_serverLocation(NULL)
    , m_status(Http::Status::OK)
    , m_timeout(30) // 30 seconds default timeout
    , m_parsingState(IDLE)
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
    {}

Request&
Request::operator=(const Request& copy)
{
    if (this == &copy)
        return (*this);

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

    return (*this);
}

Http::Status::Number
Request::mf_handleStreamedBody(const std::string& rawData)
{
    m_status = mf_parseBody(rawData);
    if (m_status != Http::Status::OK) {
        m_parsingState = ERROR;
    }

    return m_status;
}

Http::Status::Number
Request::mf_handlePostBody(const std::string& rawData, size_t bodyStart)
{
    m_status = mf_parseBody(rawData.substr(bodyStart));
    if (m_status != Http::Status::OK) {
        m_parsingState = ERROR;
        return m_status;
    }

    return m_status;
}

Http::Status::Number
Request::mf_parseFirstIncomming(const std::string& rawData)
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
        return m_status;
    }

    m_parsingState = COMPLETED;
    return m_status;
}


// NOT IMPLEMENTED YET
void    Request::reset()
{
    // this was generated by copilot, trust none of it

    //m_method.clear();
    //m_uri.clear();
    //m_httpVersion.clear();
    //m_headers.clear();
    //m_uriComponents.clear();
    //m_body.clear();
    //m_status = Http::Status::OK;
    //m_parsingState = IDLE;
}

// NOT IMPLEMENTED YET
int Request::parse(const BaseBuffer& buffer)
{
    std::string stringBuffer;
    BufferView(buffer.data(), buffer.size()).to_string(stringBuffer);
    return (this->parse(stringBuffer));
}

int Request::parse(const std::string& rawData)
{
    try {
        if (m_parsingState == IDLE) {
            m_status = mf_parseFirstIncomming(rawData);
        }

        else if (m_parsingState == INCOMPLETE) {
            m_status = mf_handleStreamedBody(rawData);
        }

        else if (m_parsingState == ERROR || m_parsingState == COMPLETED) {
            return m_status;
        }
    }
    catch (const std::exception& e) {
        m_parsingState = ERROR;
        m_status = Http::Status::INTERNAL_ERROR;
    }

    return m_status;
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
    return (m_parsingState == STARTED);
}

bool Request::isError() const
{
    return (m_parsingState == ERROR);
}

bool Request::isIncomplete() const
{
    return (m_parsingState == INCOMPLETE);
}

bool Request::isCompleted() const
{
    return (m_parsingState == COMPLETED);
}

} // namespace Http