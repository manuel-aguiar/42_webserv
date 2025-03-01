

# include "HttpCgiGateway.hpp"
# include "CgiHandlers.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"
# include "../../Connections/Connection/Connection.hpp"

# include <unistd.h> // write

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
		, m_processHttpBody(&CgiGateway::mf_HttpBodyNone)
		, m_fillFunction(&CgiGateway::mf_fillNothingToSend) {}

	void
	CgiGateway::close()
	{
		if (m_cgiRequest)
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
		m_processHttpBody = &CgiGateway::mf_HttpBodyNone;
		m_fillFunction = &CgiGateway::mf_fillNothingToSend;
	}
		
	CgiGateway::~CgiGateway() {}
	CgiGateway::CgiGateway(const CgiGateway& other)
		: m_module(other.m_module)
		, m_canRead(other.m_canRead)
		, m_canWrite(other.m_canWrite)
		, m_readFd(other.m_readFd)
		, m_writeFd(other.m_writeFd)
		, m_statusCode(other.m_statusCode)
		, m_headers(other.m_headers)
		, m_currentHeader(other.m_currentHeader)
		, m_processHttpBody(other.m_processHttpBody)
		, m_fillFunction(other.m_fillFunction) {}

	CgiGateway&
	CgiGateway::operator=(const CgiGateway& other)
	{
		if (this == &other)
			return (*this);
		
		ASSERT_EQUAL(&m_module, &other.m_module, "CgiGateway::operator=() - module mismatch");

		m_canRead = other.m_canRead;
		m_canWrite = other.m_canWrite;
		m_readFd = other.m_readFd;
		m_writeFd = other.m_writeFd;
		m_statusCode = other.m_statusCode;
		m_headers = other.m_headers;
		m_currentHeader = other.m_currentHeader;
		m_processHttpBody = other.m_processHttpBody;
		m_fillFunction = other.m_fillFunction;

		return (*this);
	}	
}
