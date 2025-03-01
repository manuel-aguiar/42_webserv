

# include "HttpCgiGateway.hpp"
# include "CgiHandlers.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"
# include "../../Connections/Connection/Connection.hpp"

extern int checkForbiddenHeaders(const std::vector<Cgi::Header>& headers);
extern bool isHeaderIgnored(const Cgi::Header& header);
extern const char* getStatusMessage(int statusCode);

namespace Http
{
	CgiGateway::CgiGateway(Cgi::Module& module)
		: m_module(module)
		, m_cgiRequest(NULL)
		, m_canRead(false)
		, m_canWrite(false)
		, m_readFd(Ws::FD_NONE)
		, m_writeFd(Ws::FD_NONE) 
		, m_statusCode(-1)
		, m_headers(NULL)
		, m_currentHeader(-1)
		, m_fillFunction(&CgiGateway::mf_fillNothingToSend) {}


	void
	CgiGateway::onCgiSuccess()
	{
		// dunno yet
	}

	void
	CgiGateway::onCgiError()
	{
		m_statusCode = Http::Status::INTERNAL_ERROR;
		m_fillFunction = &CgiGateway::mf_fillErrorResponse;
	}

	Cgi::IO::State
	CgiGateway::onCgiRead(const Ws::fd readFd)
	{
		m_readFd = readFd;
		m_canRead = true;
		return (Cgi::IO::CONTINUE);
	}

	Cgi::IO::State
	CgiGateway::onCgiWrite(const Ws::fd writeFd)
	{
		m_writeFd = writeFd;
		m_canWrite = true;
		return (Cgi::IO::CONTINUE);
	}

	Cgi::IO::State
	CgiGateway::onCgiReceiveHeaders(const Cgi::HeaderData& headers)
	{
		m_statusCode = headers.getStatusCode();
		m_headers = &headers;

		if (!checkForbiddenHeaders(headers.getHeaders()))
		{
			m_cgiRequest->setNotify_onError(NULL);	//disable error notification from premature closure
			m_module.finishRequest(*m_cgiRequest, true);
			m_statusCode = Http::Status::BAD_GATEWAY;
			m_fillFunction = &CgiGateway::mf_fillErrorResponse;
			return (Cgi::IO::CLOSE);
		}
		m_fillFunction = &CgiGateway::mf_fillResponseLine;

		return (Cgi::IO::CONTINUE);
	}

	bool
	CgiGateway::initiateRequest(const Http::ResponseData& responseData)
	{
		const Http::RequestData& data = *responseData.requestData;

		Http::RequestData::headerContainer::const_iterator finder;

		m_cgiRequest = m_module.acquireRequest();

		if (!m_cgiRequest)
			return (false);
		
		m_cgiRequest->setUser(this);
		m_cgiRequest->setNotify_onSuccess(&CgiHandlers::onSuccess);
		m_cgiRequest->setNotify_onError(&CgiHandlers::onError);
		m_cgiRequest->setWriteToScript_Callback(&CgiHandlers::onWrite);
		m_cgiRequest->setReadBodyFromScript_Callback(&CgiHandlers::onRead);
		m_cgiRequest->setReceiveStatusHeaders_Callback(&CgiHandlers::onReceiveHeaders);

		// looking at location to find the interpreter
		// m_cgiRequest->setInterpreterPath(interpreterPath); // hardcoded for now


		// CONTENT-LENGTH
		finder = data.headers.find("Content-Length");
		if (finder != data.headers.end())
			m_cgiRequest->setEnvBase(Cgi::Env::Enum::CONTENT_LENGTH, finder->second);

		// CONTENT-TYPE
		finder = data.headers.find("Content-Type");
		if (finder != data.headers.end())
			m_cgiRequest->setEnvBase(Cgi::Env::Enum::CONTENT_TYPE, finder->second);

		// GATEWAY_INTERFACE CGI/1.1
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::GATEWAY_INTERFACE, "CGI/1.1");
		
		// REMOTE_ADDR -> from the connection
		//if (connection)
		//{
		//	m_cgiRequest->setEnvBase(Cgi::Env::Enum::SERVER_PORT, 
		//		StringUtils::to_string(::ntohl(connection->info_getPeerInfo().addr.sockaddr_in.sin_addr.s_addr)));
		//}

		// PATH_INFO
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::PATH_INFO, data.path);

		// PATH_TRANSLATED

		
		// QUERY_STRING
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::QUERY_STRING, data.queryString);
		
		// REQUEST_METHOD -> HttpRequestData
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::REQUEST_METHOD, data.method);
		
		// SCRIPT_NAME	-> script
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::SCRIPT_NAME, responseData.targetPath);

		// SERVER_NAME -> webserv
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::SERVER_NAME, "42_webserv");

		// SERVER_PORT -> from connection
		//if (connection)
		//{
		//	m_cgiRequest->setEnvBase(Cgi::Env::Enum::SERVER_PORT, 
		//		StringUtils::to_string(::ntohs(connection->info_getPeerInfo().addr.sockaddr_in.sin_port)));
		//}

		// SERVER_PROTOCOL HTTP/1.1
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::SERVER_PROTOCOL, "HTTP/1.1");
		
		// SERVER_SOFTWARE Webserv
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::SERVER_SOFTWARE, "42_webserv");
		
		m_module.enqueueRequest(*m_cgiRequest, true);

		return (true);
	}

	void
	CgiGateway::close()
	{
		if (!m_cgiRequest)
			return ;
		m_module.finishRequest(*m_cgiRequest, true);
		reset();
	}

	void
	CgiGateway::reset()
	{
		m_cgiRequest = NULL;
		m_canRead = false;
		m_canWrite = false;
		m_readFd = Ws::FD_NONE;
		m_writeFd = Ws::FD_NONE;

		m_statusCode = -1;
		m_headers = NULL;
		m_currentHeader = -1;
		m_fillFunction = &CgiGateway::mf_fillNothingToSend;
	}

	Http::ResponseStatus::Type
	CgiGateway::fillWriteBuffer(BaseBuffer& writeBuffer)
	{
		return ((this->*m_fillFunction)(writeBuffer));
	}

	BufferView
	CgiGateway::receiveRequestBody(const BufferView& view)
	{

		(void)view;
		return (BufferView());
	}	
		
	CgiGateway::~CgiGateway() {}
	CgiGateway::CgiGateway(const CgiGateway& other)
		: m_module(other.m_module)
		, m_canRead(other.m_canRead)
		, m_canWrite(other.m_canWrite)
		, m_readFd(other.m_readFd)
		, m_writeFd(other.m_writeFd)
		, m_statusCode(other.m_statusCode)
		, m_headers(other.m_headers) {}

	CgiGateway&
	CgiGateway::operator=(const CgiGateway& other)
	{
		if (this == &other)
			return (*this);
		return (*this);
	}	
}
