/* *********************************/
/*                                 */
/*   Http::Request.cpp             */
/*   - implements the Http::Request*/
/*    class.                       */
/*                                 */
/* *********************************/

#include "HttpRequest.hpp"
#include "../HttpResponse/HttpResponse.hpp"
#include "../../ServerContext/ServerContext.hpp"
#include "../../ServerConfig/ServerConfig/ServerConfig.hpp"


# include <limits>
# include <cstdlib> //atoi
# include <algorithm> //max/min


static const BufferView contentLengthFind	("Content-Length");
static const BufferView contentTypeFind		("Content-Type");
static const BufferView transferEncodingFind("Transfer-Encoding");
static const BufferView multipartFind		("multipart/form-data");
static const BufferView multipartBoundaryFind("boundary=");
static const BufferView chunkedFind			("chunked");





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
	m_maxHeaderSize(std::numeric_limits<int>::max()),
	m_maxBodySize(std::numeric_limits<int>::max()),
	m_totalReadCounter(0),
	m_findPivot(0),
    m_curChunkSize(-1),
    m_curChunkPos(-1),
    m_curContentLength(-1),
    m_curContentPos(-1)
{
	const ServerConfig* serverConfig = m_serverContext.getServerConfig();
	if (serverConfig)
	{
		m_maxHeaderSize = serverConfig->getClientHeaderSize();
		m_maxBodySize = serverConfig->getClientBodySize();
	}
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
	m_maxHeaderSize(copy.m_maxHeaderSize),
	m_maxBodySize(copy.m_maxBodySize),
	m_totalReadCounter(copy.m_totalReadCounter),
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
	m_maxHeaderSize = copy.m_maxHeaderSize;
	m_maxBodySize = copy.m_maxBodySize;
	m_totalReadCounter = copy.m_totalReadCounter;
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
	m_totalReadCounter = 0;
	m_findPivot = 0;
    m_curChunkSize = -1;
    m_curChunkPos = -1;
    m_curContentLength = -1;
    m_curContentPos = -1;
}

/*
	@returns: BufferView of the remaining data that wasn't consumed
*/
Http::IOStatus::Type
Request::read()
{
	ASSERT_EQUAL(m_readBuffer != NULL, true, "Request::read(): request has no read buffer assigned");
	ASSERT_EQUAL(m_readFd != Ws::FD_NONE, true, "Request::read(): request has no read fd assigned");

	if (m_parsingState == COMPLETED || m_parsingState == ERROR || m_readBuffer->available() == 0)
		return (Http::IOStatus::WAITING);

	int readBytes = m_readBuffer->read(m_readFd, m_readBuffer->size() == m_readBuffer->capacity() ? 0 : m_readBuffer->size());
	if (readBytes <= 0)
		return (Http::IOStatus::MARK_TO_CLOSE);
	return (Http::IOStatus::WAITING);
}


Http::IOStatus::Type
Request::parse()
{
	ASSERT_EQUAL(m_readBuffer != NULL, true, "Request::parse(): request has no read buffer assigned");

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
		return (Http::IOStatus::MARK_TO_CLOSE);
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
