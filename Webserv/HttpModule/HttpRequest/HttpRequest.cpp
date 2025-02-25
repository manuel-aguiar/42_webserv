/* *********************************/
/*                                 */
/*   Http::Request.cpp             */
/*   - implements the Http::Request*/
/*    class.                       */
/*                                 */
/* *********************************/

#include "HttpRequest.hpp"
#include "../HttpResponse/HttpResponse.hpp"

#include <cstdlib> //atoi

namespace Http
{

Request::Request(ServerContext &serverContext):
	m_serverContext(serverContext),
	m_response(NULL),
	m_parsingState(IDLE),
	m_parsingFunction(&Request::mf_handleNothing),
	m_data(),
    m_curChunkSize(-1),
    m_curChunkPos(-1),
    m_curContentLength(-1),
    m_curContentPos(-1)
{

}

Request::~Request(){}

Request::Request(const Request& copy):
	m_serverContext(copy.m_serverContext),
	m_response(copy.m_response),
	m_parsingState(copy.m_parsingState),
	m_parsingFunction(copy.m_parsingFunction),
	m_data(copy.m_data),
    m_curChunkSize(copy.m_curChunkSize),
    m_curChunkPos(copy.m_curChunkPos),
    m_curContentLength(copy.m_curContentLength),
    m_curContentPos(copy.m_curContentPos)
{
	*this = copy;
}

Request&
Request::operator=(const Request& copy)
{
	if (this == &copy)
		return (*this);

	m_response = copy.m_response;
	m_data = copy.m_data;
	m_parsingState = copy.m_parsingState;
	m_parsingFunction = copy.m_parsingFunction;
    m_curChunkSize = copy.m_curChunkSize;
    m_curChunkPos = copy.m_curChunkPos;
    m_curContentLength = copy.m_curContentLength;
    m_curContentPos = copy.m_curContentPos;

	return (*this);
}

void
Request::reset()
{
	m_data.reset();
	m_parsingState = IDLE;
	m_parsingFunction = &Request::mf_handleNothing;
    m_curChunkSize = -1;
    m_curChunkPos = -1;
    m_curContentLength = -1;
    m_curContentPos = -1;
}

/*
	Buffer is not const anymore,
	if there is not enough data, Request will
	buffer::truncatePush the extra data back to the beginning.
	Buffer will be read again, and "complete" the request.
*/
void
Request::parse(BaseBuffer& buffer)
{
	BufferView currentView(buffer.data(), buffer.size());

	//std::cout << "parse: view size: " << currentView.size() << std::endl;
	//std::cout << "parse: view content: '" << currentView << "'" << std::endl;

	try
	{
		(void)((this->*m_parsingFunction)(buffer, currentView));
	}
	catch (const std::exception& e) {
		m_parsingState = ERROR;
		m_data.status = Http::Status::INTERNAL_ERROR;
		m_response->receiveRequestData(m_data);             //blew up, tell response to inform
	}
}

BufferView Request::mf_handleNothing(BaseBuffer& buffer, const BufferView& currentView)
{
	if (m_parsingState != IDLE)
		return (currentView); // already started, do nothing
	
	m_parsingState = REQLINE;
	m_parsingFunction = &Request::mf_handleRequestLine;
	return (mf_handleRequestLine(buffer, currentView));
}

BufferView Request::mf_handleRequestLine(BaseBuffer& buffer, const BufferView& receivedView)
{
	BufferView currentView = receivedView;

	if (receivedView.size() == 0)
		return (BufferView()); // not enough to go through yet

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

	m_data.status = mf_parseRequestLine(requestLine);							// call parser, error checking


	if (m_data.status != Http::Status::OK)
	{
		m_parsingState = ERROR;
		if (m_response)
			m_response->receiveRequestData(m_data);                 // inform response right away
		return (BufferView());
	}

	// transition to headers
	m_parsingState = HEADERS;

	m_parsingFunction = &Request::mf_handleHeaders; // next handler is headers
	return (mf_handleHeaders(buffer, currentView));
}

BufferView Request::mf_handleHeaders(BaseBuffer& buffer, const BufferView& receivedView)
{
	BufferView currentView = receivedView;

	if (receivedView.size() == 0)
		return (BufferView()); // not enough to go through yet

	while (currentView.size() > 0 && m_parsingState == HEADERS)
	{
		//std::cout << "\t\t current size " << currentView.size() << ", view: '" << currentView << "'" << std::endl;
		size_t headerEnd = currentView.find("\r\n", 2, 0);
		if (headerEnd == BufferView::npos)
		{
			// HARD LIMIT, single header size cannot be bigger than the buffer capacity
			if (currentView.size() >= buffer.capacity())
			{
				m_parsingState = ERROR;
				m_data.status = Http::Status::REQUEST_HEADER_FIELDS_TOO_LARGE;      // HEADER too long, AMMEND ERROR CODE
				m_parsingFunction = &Request::mf_handleNothing; // next handler is nothing
				if (m_response)
				{
					m_response->receiveRequestData(m_data);
				}
				//std::cout << "header size too big" << std::endl;
			}
			else
				buffer.truncatePush(currentView); // push the remaining data back to the beginning
			
			return (BufferView()); // not enough to go through yet
		}
		if (headerEnd == 0)
		{
			// \r\n found at the beginning: end of headers, move to BODY
			currentView = currentView.substr(2, currentView.size() - 2); // move to body
			m_parsingState = BODY;
			mf_prepareBodyParser(); // next handler is body
            break ;
		}
		BufferView thisHeader = currentView.substr(0, headerEnd); // segregate this header
		currentView = currentView.substr(headerEnd + 2, currentView.size() - headerEnd - 2); // move to next header

		// parse this header
		m_data.status = mf_parseHeaders(thisHeader);
		if (m_data.status != Http::Status::OK)
		{
			m_parsingState = ERROR;
			m_parsingFunction = &Request::mf_handleNothing; // next handler is nothing
			break ;
		}        
	}

	if (currentView.size() == 0)
		buffer.clear();

	// send right away
	if (m_parsingState != HEADERS) // if not parsing headers anymore (now body, error, etc), headers are complete, move on
	{
		if (m_response)
		{
			m_response->receiveRequestData(m_data);
		}
	}

	//std::cout << "ready to call body parser" << std::endl;
	// either nothing or body
	return ((this->*m_parsingFunction)(buffer, currentView)); // return buffer view of our current position, for the next parser
}

/*

    File Upload: multipart, has a route and a file name
    Content-Disposition: form-data; name="file"; filename="file.txt"


    POST /upload, sends a message body to /upload, but that is not an actual upload to a file.
    The response

*/

// UNDER REVIEW
void Request::mf_prepareBodyParser()
{

	if (m_data.method != "POST")
	{
		m_parsingState = COMPLETED; // no body needed for non POST requests
		m_parsingFunction = &Request::mf_handleNothing; // next handler is nothing
        return ;
	}
    headerContainer::iterator contentLength = m_data.headers.find("Content-Length");
    headerContainer::iterator transferEncoding = m_data.headers.find("Transfer-Encoding");
    //headerContainer::iterator contentType = m_data.headers.find("Content-Type");
    
    if ((contentLength == m_data.headers.end() && transferEncoding == m_data.headers.end())
    || (contentLength != m_data.headers.end() && transferEncoding != m_data.headers.end()))
    {
        m_parsingState = ERROR;
        m_data.status = Http::Status::BAD_REQUEST;
        m_parsingFunction = &Request::mf_handleNothing; // next handler is nothing
        if (m_response)
            m_response->receiveRequestBody(BufferView()); // send empty body, signals end of body
        return ;
    }
    if (contentLength != m_data.headers.end())
    {
        m_curContentLength = std::atoi(contentLength->second.c_str());
        m_curContentPos = 0;
        m_parsingFunction = &Request::mf_parseRegularBody;
    }
    else if (transferEncoding->second == "chunked")
    {
        m_parsingFunction = &Request::mf_parseChunkedBody;
    }
    else
    {
        m_parsingState = ERROR;
        m_data.status = Http::Status::BAD_REQUEST;      // UNSUPPORTED TRANSFER ENCODING
        m_parsingFunction = &Request::mf_handleNothing; // next handler is nothing
        if (m_response)
            m_response->receiveRequestBody(BufferView()); 
    }

	//std::cout << "Body parser prepared" << std::endl;

    return ;
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

const Http::RequestData& Request::getData() const
{
	return (m_data);
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
