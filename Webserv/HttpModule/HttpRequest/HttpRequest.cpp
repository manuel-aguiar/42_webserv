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


static const char* contentLengthFind = "Content-Length";
static const char* contentTypeFind = "Content-Type";
static const char* transferEncodingFind = "Transfer-Encoding";
static const char* multipartFind = "multipart/form-data";
static const char* multipartBoundaryFind = "boundary=";
static const char* chunkedFind = "chunked";

namespace Http
{

Request::Request(ServerContext &serverContext):
	m_serverContext(serverContext),
	m_httpResponse(NULL),
	m_readBuffer(NULL),
	m_readFd(Ws::FD_NONE),
	m_parsingState(IDLE),
	m_parsingFunction(&Request::mf_handleNothing),
	m_data(),
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
	m_httpResponse(copy.m_httpResponse),
	m_readBuffer(copy.m_readBuffer),
	m_readFd(copy.m_readFd),
	m_parsingState(copy.m_parsingState),
	m_parsingFunction(copy.m_parsingFunction),
	m_data(copy.m_data),
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

	m_httpResponse = copy.m_httpResponse;
	m_readBuffer = copy.m_readBuffer;
	m_readFd = copy.m_readFd;
	m_data = copy.m_data;
	m_parsingState = copy.m_parsingState;
	m_parsingFunction = copy.m_parsingFunction;
	m_findPivot = copy.m_findPivot;
    m_curChunkSize = copy.m_curChunkSize;
    m_curChunkPos = copy.m_curChunkPos;
    m_curContentLength = copy.m_curContentLength;
    m_curContentPos = copy.m_curContentPos;

	return (*this);
}

void
Request::close()
{
	reset();
	m_readBuffer = NULL;
	m_readFd = Ws::FD_NONE;
}

void
Request::reset()
{
	m_data.reset();
	m_parsingState = IDLE;
	m_parsingFunction = &Request::mf_handleNothing;
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

Http::IOStatus::Type
Request::read()
{
	ASSERT_EQUAL(m_readBuffer != NULL, true, "Request::read(): request has no read buffer assigned");
	ASSERT_EQUAL(m_readFd != Ws::FD_NONE, true, "Request::read(): request has no read fd assigned");

	if (m_parsingState == COMPLETED || m_readBuffer->available() == 0)
		return (Http::IOStatus::WAITING);

	int readBytes = m_readBuffer->read(m_readFd, m_readBuffer->size() == m_readBuffer->capacity() ? 0 : m_readBuffer->size());

	if (!readBytes)
		return (Http::IOStatus::MARK_TO_CLOSE);
	return (Http::IOStatus::WAITING);
}


Http::IOStatus::Type
Request::parse()
{
	ASSERT_EQUAL(m_readBuffer != NULL, true, "Request::parse(): request has no read buffer assigned");
	//std::cout << "parse request" << std::endl;

	bool cgiPass = (m_httpResponse && m_httpResponse->getResponseData().cgiPass == true);
	if (cgiPass && m_parsingState == BODY)
		return (Http::IOStatus::WAITING);

	return (mf_innerParse());
}

Http::IOStatus::Type
Request::mf_innerParse()
{
	BufferView remaining(m_readBuffer->data(), m_readBuffer->size());

	try
	{
		remaining = ((this->*m_parsingFunction)(remaining));
		m_readBuffer->truncatePush(remaining);
	}
	catch (const std::exception& e)
	{
		m_parsingState = ERROR;
		m_data.status = Http::Status::INTERNAL_ERROR;
		m_httpResponse->receiveRequestData(m_data);             //blew up, tell response to inform
	}
	return (Http::IOStatus::WAITING);
}

//parse unrestricted
Http::IOStatus::Type
Request::forceParse()
{
	ASSERT_EQUAL(m_readBuffer != NULL, true, "Request::parse(): request has no read buffer assigned");

	return (mf_innerParse());
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
BufferView Http::Request::mf_parseBodyExitError(const BufferView& remaining, const Http::Status::Number status)
{
	(void)remaining;
    m_parsingState = ERROR;
    m_data.status = status;

    if (m_httpResponse)
        m_httpResponse->receiveRequestBody(BufferView()); // send "eof" to signal end of body

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
		if (remaining.size() >= m_readBuffer->capacity())
		{
			m_parsingState = ERROR;
			m_data.status = Http::Status::URI_TOO_LONG;      // URI too long
			m_httpResponse->receiveRequestData(m_data);         // inform response right away
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
		if (m_httpResponse)
			m_httpResponse->receiveRequestData(m_data);                 // inform response right away
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
			if (remaining.size() >= m_readBuffer->capacity())
			{
				return mf_handleExitFailure(remaining, Http::Status::REQUEST_HEADER_FIELDS_TOO_LARGE);
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

			if (m_httpResponse)
				m_httpResponse->receiveRequestData(m_data);

            return (Request::mf_prepareBodyParser(remaining));
		}
		BufferView thisHeader = remaining.substr(0, headerEnd).trim(" \r\v\t\n"); // segregate this header

		//std::cout << "\t this header line: '" << thisHeader << "'" << std::endl;

		remaining = remaining.substr(headerEnd + 2, remaining.size() - headerEnd - 2); // move to next header

		// parse this header
		m_data.status = mf_parseHeaders(thisHeader);
		if (m_data.status != Http::Status::OK)
			return mf_handleExitFailure(remaining, m_data.status);
	}

	return (remaining);
}

/*

    File Upload: multipart, has a route and a file name
    Content-Disposition: form-data; name="file"; filename="file.txt"


    POST /upload, sends a message body to /upload, but that is not an actual upload to a file.
    The response

*/

// UNDER REVIEW
BufferView
Request::mf_prepareBodyParser(const BufferView& receivedView)
{
	#ifndef NDEBUG
		// making sure everything is correctly formatted
		std::string test = contentLengthFind;
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToCapitalized() == BufferView(contentLengthFind), true, "contentLengthFind is not correctly formated");
		test = contentTypeFind;
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToCapitalized() == BufferView(contentTypeFind), true, "contentTypeFind is not correctly formated");
		test = transferEncodingFind;
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToCapitalized() == BufferView(transferEncodingFind), true, "transferEncodingFind is not correctly formated");
		test = multipartFind;
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToLowerCase() == BufferView(multipartFind), true, "multipartFind is not correctly formated");
		test = multipartBoundaryFind;
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToLowerCase() == BufferView(multipartBoundaryFind), true, "multipartBoundaryFind is not correctly formated");
	#endif

    RequestData::headerContainer::iterator contentLength = m_data.headers.find(contentLengthFind);
    RequestData::headerContainer::iterator transferEncoding = m_data.headers.find(transferEncodingFind);
    RequestData::headerContainer::iterator contentType = m_data.headers.find(contentTypeFind);
    bool isCgi = (m_httpResponse && m_httpResponse->getResponseData().responseType == Http::ResponseData::CGI);

	if (contentLength == m_data.headers.end() 
	&& transferEncoding == m_data.headers.end())
	{
		if (m_data.method == "POST")
			return (mf_parseBodyExitError(receivedView, Http::Status::LENGTH_REQUIRED));

		m_parsingState = COMPLETED;
		m_parsingFunction = &Request::mf_handleNothing;
		if (m_httpResponse)
			m_httpResponse->receiveRequestBody(BufferView()); // send empty buffer, end of message body
	}

    else if (contentLength != m_data.headers.end() && transferEncoding != m_data.headers.end())
	{
		return (mf_parseBodyExitError(receivedView, Http::Status::BAD_REQUEST));
	}

    else if (contentLength != m_data.headers.end())
    {
        m_curContentLength = std::atoi(contentLength->second.c_str());
        m_curContentPos = 0;
        m_parsingFunction = &Request::mf_parseLengthBody;
		if (contentType != m_data.headers.end()
		&& contentType->second.find(multipartFind) != std::string::npos)
		{
			size_t boundaryPos = contentType->second.find(multipartBoundaryFind);
			if (boundaryPos == std::string::npos)
				return (mf_parseBodyExitError(receivedView, Http::Status::BAD_REQUEST));
			m_data.multipart_Boundary = contentType->second.substr(boundaryPos + 9);
			m_parsingFunction = &Request::mf_parseMultipartBody_Start;
			
			// if response is CGI, wait for it to signal that is time to parse
			if (isCgi)
				m_parsingFunction = &Request::mf_parseLengthBody; // cgi treats multipart as regular
		}
    }
    else if (transferEncoding->second == chunkedFind)
        m_parsingFunction = &Request::mf_parseChunkedBody_GetChunk;
    else
		return (mf_parseBodyExitError(receivedView, Http::Status::NOT_IMPLEMENTED));

	// if cgi stop the parsing, wait for cgi to signal
	if (isCgi)
		return (receivedView);

	return ((this->*m_parsingFunction)(receivedView));
}

void Request::mf_handleExitFailure(Http::Status::Number status)
{
    m_parsingState = ERROR;
    m_data.status = status;
    m_parsingFunction = &Request::mf_handleNothing;
    if (m_httpResponse) {
		m_httpResponse->receiveRequestData(m_data);
	}

	return;
}

BufferView Request::mf_handleExitFailure(BufferView& remaining, Http::Status::Number status)
{
	mf_handleExitFailure(status);
	return (remaining);
}

void
Request::setBuffer_ReadFd(BaseBuffer& buffer, const Ws::fd fd)
{
	m_readBuffer = &buffer;
	m_readFd = fd;
}

// setters
void Request::setResponse(Http::Response& response)
{
	m_httpResponse = &response;
}

// Getters
const Request::ParsingState& Request::getParsingState() const
{
	return (m_parsingState);
}

Http::Response& Request::getResponse()
{
	return (*m_httpResponse);
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
