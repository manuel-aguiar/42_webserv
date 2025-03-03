

# include "HttpCgiResponse.hpp"
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
	CgiResponse::CgiResponse(Cgi::Module& module)
		: m_module(module)
		, m_cgiRequest(NULL)
		, m_canRead(false)
		, m_canWrite(false)
		, m_readFd(Ws::FD_NONE)
		, m_writeFd(Ws::FD_NONE)
		, m_responseData(NULL)
		, m_file()
		, m_staticReadCounter(0)
		, m_defaultErrorPage()
		, m_statusCode(-1)
		, m_headers(NULL)
		, m_tempBody()	
		, m_currentHeader(-1)
		, m_processHttpBody(&CgiResponse::mf_HttpBodyNone)
		, m_fillFunction(&CgiResponse::mf_fillNothingToSend) {}

	void
	CgiResponse::close()
	{
		mf_finishAndRelease();
		reset();
	}

	void
	CgiResponse::mf_finishAndRelease()
	{
		if (m_cgiRequest)
			m_module.finishRequest(*m_cgiRequest, true);
		m_cgiRequest = NULL;
	}

	void
	CgiResponse::reset()
	{
		m_cgiRequest = NULL;
		m_canRead = false;
		m_canWrite = false;
		m_readFd = Ws::FD_NONE;
		m_writeFd = Ws::FD_NONE;
		m_responseData = NULL;
		m_file.reset();
		m_staticReadCounter = 0;
		m_defaultErrorPage.clear();
		m_statusCode = -1;
		m_headers = NULL;
		m_tempBody = BufferView();
		m_currentHeader = -1;
		m_processHttpBody = &CgiResponse::mf_HttpBodyNone;
		m_fillFunction = &CgiResponse::mf_fillNothingToSend;
	}
		
	CgiResponse::~CgiResponse() { close();}
	CgiResponse::CgiResponse(const CgiResponse& other)
		: m_module(other.m_module)
		, m_canRead(other.m_canRead)
		, m_canWrite(other.m_canWrite)
		, m_readFd(other.m_readFd)
		, m_writeFd(other.m_writeFd)
		, m_responseData(other.m_responseData)
		, m_staticReadCounter(other.m_staticReadCounter)
		, m_defaultErrorPage(other.m_defaultErrorPage)
		, m_statusCode(other.m_statusCode)
		, m_headers(other.m_headers)
		, m_tempBody()
		, m_currentHeader(other.m_currentHeader)
		, m_processHttpBody(other.m_processHttpBody)
		, m_fillFunction(other.m_fillFunction) {}

	CgiResponse&
	CgiResponse::operator=(const CgiResponse& other)
	{
		if (this == &other)
			return (*this);
		
		ASSERT_EQUAL(&m_module, &other.m_module, "CgiGateway::operator=() - module mismatch");

		m_canRead = other.m_canRead;
		m_canWrite = other.m_canWrite;
		m_readFd = other.m_readFd;
		m_writeFd = other.m_writeFd;
		m_responseData = other.m_responseData;
		m_staticReadCounter = other.m_staticReadCounter;
		m_defaultErrorPage = other.m_defaultErrorPage;
		m_statusCode = other.m_statusCode;
		m_headers = other.m_headers;
		m_tempBody = other.m_tempBody;
		m_currentHeader = other.m_currentHeader;
		m_processHttpBody = other.m_processHttpBody;
		m_fillFunction = other.m_fillFunction;

		return (*this);
	}	
}
