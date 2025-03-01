


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


	void
	CgiGateway::onCgiSuccess()
	{
		// dunno yet
	}

	void
	CgiGateway::onCgiError()
	{
        //std::cout << "onerror received" << std::endl;
		// must check if i am already sending data back
        //std::cout << "received errpor" << std::endl;
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

		// not sending anything anymore, may close write
		if (m_processHttpBody != &CgiGateway::mf_HttpBodySend)
		{
			m_writeFd = Ws::FD_NONE;			
			m_canWrite = false;
			return (Cgi::IO::CLOSE);
		}
		return (Cgi::IO::CONTINUE);
	}

	Cgi::IO::State
	CgiGateway::onCgiReceiveHeaders(const Cgi::HeaderData& headers)
	{
		m_statusCode = headers.getStatusCode();
		m_headers = &headers;
        //std::cout << "Received headers, status code: " << m_statusCode << std::endl;
		if (!checkForbiddenHeaders(headers.getHeaders()))
		{
			m_cgiRequest->setNotify_onError(NULL);	//disable error notification from premature closure
			m_module.finishRequest(*m_cgiRequest, true);
			m_statusCode = Http::Status::BAD_GATEWAY;
			m_fillFunction = &CgiGateway::mf_fillErrorResponse;
			return (Cgi::IO::CLOSE);
		}
        //std::cout << "filling response line?" << std::endl;
		m_fillFunction = &CgiGateway::mf_fillResponseLine;

		return (Cgi::IO::CONTINUE);
	}
}