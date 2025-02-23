

# include "HttpCgiGateway.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"

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
	CgiGateway::onSuccess()
	{
		// dunno yet
	}

	void
	CgiGateway::onError()
	{
		m_statusCode = Http::Status::INTERNAL_ERROR;
		m_fillFunction = &CgiGateway::mf_fillErrorResponse;
	}

	Cgi::IO::State
	CgiGateway::onRead(const Ws::fd readFd)
	{
		m_readFd = readFd;
		m_canRead = true;
		return (Cgi::IO::CONTINUE);
	}

	Cgi::IO::State
	CgiGateway::onWrite(const Ws::fd writeFd)
	{
		m_writeFd = writeFd;
		m_canWrite = true;
		return (Cgi::IO::CONTINUE);
	}

	Cgi::IO::State
	CgiGateway::onReceiveHeaders(const Cgi::HeaderData& headers)
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

	Http::ResponseStatus
	CgiGateway::fillWriteBuffer(BaseBuffer& writeBuffer)
	{
		return ((this->*m_fillFunction)(writeBuffer));
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