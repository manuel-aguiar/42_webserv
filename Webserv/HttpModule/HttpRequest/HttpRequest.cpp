/* *********************************/
/*                                 */
/*   Http::Request.cpp             */
/*   - implements the Http::Request*/
/*    class.                       */
/*                                 */
/* *********************************/

#include "HttpRequest.hpp"
#include "../HttpResponse/HttpResponse.hpp"

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

/*
    Buffer is not const anymore,
    if there is not enough data, Request will
    buffer::truncatePush the extra data back to the beginning.
    Buffer will be read again, and "complete" the request.
*/
void Request::parse(BaseBuffer& buffer)
{
    BufferView currentView(buffer.data(), buffer.size());

    try
    {
        switch (m_parsingState)
        {
            case IDLE:
                m_parsingState = REQLINE;
                // intentional fallthrough

            case REQLINE:
            {
                currentView = mf_handleRequestLine(buffer, currentView);    // move the current view as you go
                if (m_parsingState == ERROR || m_parsingState == REQLINE)
                    return;
                // intentional fallthrough
            }

            case HEADERS:
            {
                currentView = mf_handleHeaders(buffer, currentView);        // move the current view as you go
                if (m_parsingState == ERROR || m_parsingState == HEADERS)
                    return;
                // intentional fallthrough
            }

            case BODY:
            {

                //NOT IMPLEMENTED

                mf_handleBody(buffer, currentView);
                if (m_parsingState == ERROR || m_parsingState == BODY)
                    return;
            }

            case ERROR:
            case COMPLETED:
                return;
        }
    }
    catch (const std::exception& e) {
        m_parsingState = ERROR;
        m_data.status = Http::Status::INTERNAL_ERROR;
        m_response->receiveRequestData(m_data);             //blew up, tell response to inform
    }
}

BufferView Request::mf_handleRequestLine(BaseBuffer& buffer, const BufferView& receivedView)
{
    BufferView currentView = receivedView;

    size_t reqLineEnd = currentView.find("\r\n", 2, 0);
    if (reqLineEnd == std::string::npos)
    {
        // HARD LIMIT, request line cannot be bigger than buffer size
        if (currentView.size() >= buffer.capacity())
        {
            m_parsingState = ERROR;
            m_data.status = Http::Status::BAD_REQUEST;      // URI too long, AMMEND ERROR CODE
            m_response->receiveRequestData(m_data);         // inform response right away
        }
        else
            buffer.truncatePush(currentView);                   // push the remaining data back to the beginning
        return (BufferView()); // not enough to go through yet
    }

    BufferView requestLine(currentView.substr(0, reqLineEnd));
    currentView = currentView.substr(reqLineEnd + 2, currentView.size() - reqLineEnd - 2); // move view forward

    m_data.status = mf_parseRequestLine(requestLine);
    if (m_data.status != Http::Status::OK)
    {
        m_parsingState = ERROR;
        m_response->receiveRequestData(m_data);                 // inform response right away
        return (BufferView());
    }

    // transition to headers
    m_parsingState = HEADERS;

    // return current view to where headers start
    return (currentView);
}

BufferView Request::mf_handleHeaders(BaseBuffer& buffer, const BufferView& receivedView)
{
    BufferView currentView = receivedView;

    while (currentView.size() > 0 && m_parsingState == HEADERS)
    {
        size_t headerEnd = currentView.find("\r\n", 2, 0);
        if (headerEnd == BufferView::npos)
        {
            // HARD LIMIT, single header size cannot be bigger than the buffer capacity
            if (currentView.size() >= buffer.capacity())
            {
                m_parsingState = ERROR;
                m_data.status = Http::Status::REQUEST_HEADER_FIELDS_TOO_LARGE;      // HEADER too long, AMMEND ERROR CODE
            }
            else
                buffer.truncatePush(currentView); // push the remaining data back to the beginning
            
            return (BufferView()); // not enough to go through yet
        }
        if (headerEnd == 0)
        {
            // \r\n found at the beginning: end of headers, move to BODY
            currentView = currentView.substr(2, currentView.size() - 2); // move to body
            
            
            
            
            // NO BODY, MARKING COMPLETED DIRECTLY FOR TEST PURPOSES
            
            m_parsingState = COMPLETED;
            
            
            
            
            
            break ;
        }
        BufferView thisHeader = currentView.substr(0, headerEnd); // segregate this header
        currentView = currentView.substr(headerEnd + 2, currentView.size() - headerEnd - 2); // move to next header

        // parse this header
        m_data.status = mf_parseHeaders(thisHeader);
        if (m_data.status != Http::Status::OK)
        {
            m_parsingState = ERROR;
            break ;
        }        
    }


    // send right away
    if (m_parsingState != HEADERS) // if not parsing headers anymore (now body, error, etc), headers are complete, move on
        m_response->receiveRequestData(m_data);



    return (currentView); // return buffer view of our current position, for the next parser
}


// UNDER REVIEW
BufferView Request::mf_handleBody(BaseBuffer& buffer, const BufferView& currentView)
{
    (void)buffer;
    if (m_data.method != "POST")
    {
        m_parsingState = COMPLETED; // no body needed for non POST requests
        return (BufferView());
    }

    size_t bodyStart = currentView.find("\r\n\r\n", 4, 0);
    if (bodyStart == BufferView::npos)
        return (BufferView()); // not enough to go through yet

    bodyStart += 4;

    BufferView body(currentView.data() + bodyStart, currentView.size() - bodyStart);
    m_data.status = mf_parseBody(body);
    if (m_data.status != Http::Status::OK) {
        m_parsingState = ERROR;
        return (BufferView());
    }

    // transition to completed
    m_parsingState = COMPLETED;

    // NOT IMPLEMENTED
    return (currentView);
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
