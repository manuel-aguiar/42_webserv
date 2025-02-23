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

Request::Request(ServerContext &serverContext):
     m_serverContext(serverContext),
     m_response(NULL)
{
    reset();
}

Request::~Request()
{}

Request::Request(const Request& copy):
    m_serverContext(copy.m_serverContext)
{
    *this = copy;
}

Request&
Request::operator=(const Request& copy)
{
    if (this == &copy) return (*this);

    m_serverContext = copy.m_serverContext;
    m_response = copy.m_response;
    m_parsingState = copy.m_parsingState;
    m_data.method = copy.m_data.method;
    m_data.uri = copy.m_data.uri;
    m_data.path = copy.m_data.path;
    m_data.queryString = copy.m_data.queryString;
    m_data.fragment = copy.m_data.fragment;
    m_data.httpVersion = copy.m_data.httpVersion;
    m_data.headers = copy.m_data.headers;
    m_data.body = copy.m_data.body;
    m_data.status = copy.m_data.status;
    m_data.bodyType = copy.m_data.bodyType;
    m_data.contentType = copy.m_data.contentType;
    m_data.expectedLength = copy.m_data.expectedLength;
    return (*this);
}

void  Request::reset()
{
    m_parsingState = IDLE;
    m_data.reset();
}

void Request::mf_handleRequestLine(const BufferView& buffer)
{
    size_t reqLineEnd = buffer.find("\r\n", 2, 0);
    if (reqLineEnd == std::string::npos) return; // not enough to go through yet

    BufferView requestLine(buffer.substr(0, reqLineEnd));
    m_data.status = mf_parseRequestLine(requestLine);
    if (m_data.status != Http::Status::OK) {
        m_parsingState = ERROR;
        return;
    }

    // transition to headers
    m_parsingState = HEADERS;
}

void Request::mf_handleHeaders(const BufferView& buffer)
{
    size_t headerStart = buffer.find("\r\n", 2, 0);
    if (headerStart == BufferView::npos) return; // not enough to go through yet

    size_t headerEnd = buffer.find("\r\n\r\n", 4, 0);
    if (headerEnd == BufferView::npos) return; // not enough to go through yet

    headerStart += 2;

    BufferView headers(buffer.substr(headerStart, headerEnd - headerStart));
    m_data.status = mf_parseHeaders(headers);
    if (m_data.status != Http::Status::OK) {
        m_parsingState = ERROR;
        return;
    }

    ASSERT_EQUAL(m_response != NULL, true, "Request::mf_handleHeaders(), m_response is NULL");

    // before state transition, call response to check
    m_response->receiveRequestData(m_data);

    // transition to body
    m_parsingState = BODY;
}

void Request::mf_handleBody(const BufferView& buffer)
{
    if (m_data.method != "POST") {
        m_parsingState = COMPLETED; // no body needed for non POST requests
        return;
    }

    size_t bodyStart = buffer.find("\r\n\r\n", 4, 0);
    if (bodyStart == BufferView::npos) return; // not enough to go through yet

    bodyStart += 4;

    BufferView body(buffer.data() + bodyStart, buffer.size() - bodyStart);
    m_data.status = mf_parseBody(body);
    if (m_data.status != Http::Status::OK) {
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
        m_data.status = Http::Status::INTERNAL_ERROR;
    }
}

// setters
void Request::setResponse(Http::Response& response)
{
    m_response = &response;
}

// Getters
const Request::ParsingState& Request::getParsingState() const
{
    return (m_parsingState);
}

Http::Response& Request::getResponse()
{
    return (*m_response);
}

const std::string& Request::getMethod() const
{
    return (m_data.method);
}

const std::string& Request::getUri() const
{
    return (m_data.uri);
}

const std::string& Request::getHttpVersion() const
{
    return (m_data.httpVersion);
}

const std::map<std::string, std::string>& Request::getHeaders() const
{
    return (m_data.headers);
}

const std::string& Request::getBody() const
{
    return (m_data.body);
}

const std::string& Request::getPath() const
{
    return (m_data.path);
}

const std::string& Request::getQueryString() const
{
    return (m_data.queryString);
}

const std::string& Request::getFragment() const
{
    return (m_data.fragment);
}

Http::Status::Number Request::getStatus() const
{
    return (m_data.status);
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
