


# include "HttpCgiResponse.hpp"
# include "CgiHandlers.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"
# include "../../Connections/Connection/Connection.hpp"

# include <unistd.h> // write
#include <iostream>

extern int checkForbiddenHeaders(const std::vector<Cgi::Header>& headers);
extern bool isHeaderIgnored(const Cgi::Header& header);
extern const char* getStatusMessage(int statusCode);

namespace Http
{


	void
	CgiResponse::onCgiSuccess()
	{
		// dunno yet
	}

	void
	CgiResponse::onCgiError()
	{
<<<<<<< HEAD
		mf_finishAndRelease();
=======
        //std::cout << "onerror received" << std::endl;
		// must check if i am already sending data back
        //std::cout << "received errpor" << std::endl;

		m_module.finishRequest(*m_cgiRequest, true);
>>>>>>> main
		m_statusCode = Http::Status::BAD_GATEWAY;
		if (m_fillFunction == &CgiResponse::mf_fillNothingToSend)
			m_fillFunction = &CgiResponse::mf_fillErrorResponse;
	}

	Cgi::IO::State
	CgiResponse::onCgiRead(const Ws::fd readFd)
	{
		m_readFd = readFd;
		m_canRead = true;
		if (m_readFd == Ws::FD_NONE)
			return (Cgi::IO::CLOSE);
		return (Cgi::IO::CONTINUE);
	}

	Cgi::IO::State
	CgiResponse::onCgiWrite(const Ws::fd writeFd)
	{
		m_writeFd = writeFd;
		m_canWrite = true;

		// not sending anything anymore, may close write
		if (m_processHttpBody != &CgiResponse::mf_HttpBodySend)
		{
			m_writeFd = Ws::FD_NONE;			
			m_canWrite = false;
			return (Cgi::IO::CLOSE);
		}
		return (Cgi::IO::CONTINUE);
	}

	Cgi::IO::State
	CgiResponse::onCgiReceiveHeaders(const Cgi::HeaderData& headers)
	{
		//std::cout << "received headers, code: " << headers.getStatusCode() << std::endl;
		m_statusCode = headers.getStatusCode();
		m_headers = &headers;
		m_tempBody = headers.getTempBody();
		if (m_statusCode != Cgi::RequestConsts::Status::SUCCESS
		||	!checkForbiddenHeaders(headers.getHeaders()))
		{
			m_cgiRequest->setNotify_onError(NULL);	//disable error notification from premature closure
			if (m_statusCode == Cgi::RequestConsts::Status::SUCCESS)
<<<<<<< HEAD
				mf_finishAndRelease();
=======
				m_module.finishRequest(*m_cgiRequest, true);
>>>>>>> main
			m_statusCode = Http::Status::BAD_GATEWAY;
			m_fillFunction = &CgiResponse::mf_fillErrorResponse;
			return (Cgi::IO::CLOSE);
		}
		
<<<<<<< HEAD
=======
		//std::cout << "fill response line" << std::endl;
>>>>>>> main
		m_fillFunction = &CgiResponse::mf_fillResponseLine;
		return (Cgi::IO::CONTINUE);
	}
}