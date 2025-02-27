/* *********************************/
/*                                 */
/*   Http::Request.cpp             */
/*   - implements the Http::Request*/
/*    class.                       */
/*                                 */
/* *********************************/

#include "HttpRequest.hpp"
#include "../HttpResponse/HttpResponse.hpp"

# include <cstdlib> //atoi
# include  <algorithm> //max/min

namespace Http
{

Request::Request(ServerContext &serverContext):
	m_serverContext(serverContext),
	m_response(NULL),
	m_parsingState(IDLE),
	m_parsingFunction(&Request::mf_handleNothing),
	m_data(),
	m_bufferCapacity(0),
	m_findPivot(0),
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
	m_bufferCapacity(copy.m_bufferCapacity),
	m_findPivot(copy.m_findPivot),
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
	m_bufferCapacity = copy.m_bufferCapacity;
	m_findPivot = copy.m_findPivot;
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
	m_bufferCapacity = 0;
	m_findPivot = 0;
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

/*
	@returns: BufferView of the remaining data that wasn't consumed
*/
BufferView
Request::parse(const BaseBuffer& buffer)
{
	BufferView remaining(buffer.data(), buffer.size());

	try
	{
		m_bufferCapacity = buffer.capacity();
		remaining = ((this->*m_parsingFunction)(remaining));
	}
	catch (const std::exception& e)
	{
		m_parsingState = ERROR;
		m_data.status = Http::Status::INTERNAL_ERROR;
		m_response->receiveRequestData(m_data);             //blew up, tell response to inform
	}
	return (remaining);
}

/*
	@returns: BufferView of the remaining data that wasn't consumed
*/
BufferView Request::mf_handleNothing(const BufferView& remaining)
{
	if (m_parsingState != IDLE)
		return (remaining); // already started, do nothing
	
	m_parsingState = REQLINE;
	m_parsingFunction = &Request::mf_handleRequestLine;
	return (mf_handleRequestLine(remaining));
}

/*
	@returns: BufferView of the remaining data that wasn't consumed
*/
BufferView Http::Request::mf_parseBodyExitError(const Http::Status::Number status)
{
    m_parsingState = ERROR;
    m_data.status = status;
    
    if (m_response)
        m_response->receiveRequestBody(BufferView()); // send "eof" to signal end of body
    
    m_parsingFunction = &Request::mf_handleNothing;
    return (BufferView());
}

/*
	@returns: BufferView of the remaining data that wasn't consumed
*/
BufferView Request::mf_handleRequestLine(const BufferView& receivedView)
{
	BufferView remaining = receivedView;

	if (remaining.size() == 0)
		return (BufferView());
	
	size_t reqLineEnd;
	
	BufferView target("\r\n", 2);
	reqLineEnd = remaining.find(target, m_findPivot);
	if (reqLineEnd == BufferView::npos)
	{
		m_findPivot = std::max((int)remaining.size() - (int)target.size(), 0);
		// HARD LIMIT, request line cannot be bigger than buffer size
		if (remaining.size() >= m_bufferCapacity)
		{
			m_parsingState = ERROR;
			m_data.status = Http::Status::BAD_REQUEST;      // URI too long, AMMEND ERROR CODE
			m_response->receiveRequestData(m_data);         // inform response right away
		}
		return (remaining); // not enough to go through yet
	}
	
	m_findPivot = 0;
	
	BufferView requestLine(remaining.substr(0, reqLineEnd));
	remaining = remaining.substr(reqLineEnd + 2, remaining.size() - reqLineEnd - 2); // move view forward

	m_data.status = mf_parseRequestLine(requestLine);							// call parser, error checking


	if (m_data.status != Http::Status::OK)
	{
		m_parsingState = ERROR;
		if (m_response)
			m_response->receiveRequestData(m_data);                 // inform response right away
		return (remaining);
	}

	// transition to headers
	m_parsingState = HEADERS;

	m_parsingFunction = &Request::mf_handleHeaders; // next handler is headers
	return (mf_handleHeaders(remaining));
}

BufferView Request::mf_handleHeaders(const BufferView& receivedView)
{
	BufferView remaining = receivedView;
	BufferView delimiter("\r\n", 2);

	if (receivedView.size() == 0)
		return (BufferView()); // not enough to go through yet

	while (remaining.size() > 0)
	{
		//std::cout << "\t\t current size " << remaining.size() << ", view: '" << remaining << "'" << std::endl;

		//std::cout << "lookup pivot: " << m_findPivot << std::endl;
		
		size_t headerEnd = remaining.find(delimiter, m_findPivot);
		if (headerEnd == BufferView::npos)
		{
			m_findPivot = std::max((int)remaining.size() - (int)delimiter.size(), 0);
			// HARD LIMIT, single header size cannot be bigger than the buffer capacity
			if (remaining.size() >= m_bufferCapacity)
			{
				m_data.status = Http::Status::REQUEST_HEADER_FIELDS_TOO_LARGE;      // HEADER too long, AMMEND ERROR CODE
				goto exitFailure;
			}
			return (remaining); // push the remaining data back to the beginning
		}
		
		m_findPivot = 0;

		//std::cout << "header end: " << headerEnd << std::endl;

		if (headerEnd == 0)
		{
			// \r\n found at the beginning: end of headers, move to BODY
			remaining = remaining.substr(delimiter.size(), remaining.size() - delimiter.size()); // move to body
			m_parsingState = BODY;
			mf_prepareBodyParser(); // next handler is body
			if (m_response)
				m_response->receiveRequestData(m_data);
            return ((this->*m_parsingFunction)(remaining));
		}
		BufferView thisHeader = remaining.substr(0, headerEnd); // segregate this header

		//std::cout << "\t this header line: '" << thisHeader << "'" << std::endl;

		remaining = remaining.substr(headerEnd + 2, remaining.size() - headerEnd - 2); // move to next header

		// parse this header
		m_data.status = mf_parseHeaders(thisHeader);
		if (m_data.status != Http::Status::OK)
			goto exitFailure;
	}

	return (remaining);
	
exitFailure:
	m_parsingState = ERROR;
	if (m_response)
	m_response->receiveRequestData(m_data); // inform response right away
	m_parsingFunction = &Request::mf_handleNothing; 
	return (mf_handleNothing(remaining));
	
	// return buffer view of our remaining position to the next parser
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
    RequestData::headerContainer::iterator contentLength = m_data.headers.find("Content-Length");
    RequestData::headerContainer::iterator transferEncoding = m_data.headers.find("Transfer-Encoding");
    RequestData::headerContainer::iterator contentType = m_data.headers.find("Content-Type");
    
    if ((contentLength == m_data.headers.end() && transferEncoding == m_data.headers.end())
    || (contentLength != m_data.headers.end() && transferEncoding != m_data.headers.end()))
        goto exitFailure;
    if (contentLength != m_data.headers.end())
    {
        m_curContentLength = std::atoi(contentLength->second.c_str());
        m_curContentPos = 0;
        m_parsingFunction = &Request::mf_parseRegularBody;
		if (contentType != m_data.headers.end()
		&& contentType->second.find("multipart/form-data") != std::string::npos)
		{
			size_t boundaryPos = contentType->second.find("boundary=");
			if (boundaryPos == std::string::npos)
				goto exitFailure;
			m_data.multipart_Boundary = contentType->second.substr(boundaryPos + 9);
			m_parsingFunction = &Request::mf_parseMultipartBody_Start;
		}
    }
    else if (transferEncoding->second == "chunked")
    {
        m_parsingFunction = &Request::mf_parseChunkedBody_GetChunk;
    }
    else
    {
		goto exitFailure;
    }

	//std::cout << "Body parser prepared" << std::endl;
	return ;

exitFailure:
	m_parsingState = ERROR;
	m_data.status = Http::Status::BAD_REQUEST;      // UNSUPPORTED TRANSFER ENCODING
	m_parsingFunction = &Request::mf_handleNothing; // next handler is nothing
	if (m_response)
		m_response->receiveRequestBody(BufferView()); 

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
