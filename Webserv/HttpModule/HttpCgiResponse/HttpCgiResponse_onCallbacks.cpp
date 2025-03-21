


# include "HttpCgiResponse.hpp"
# include "CgiHandlers.hpp"
# include "../HttpRequest/HttpRequest.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"
# include "../../Connections/Connection/Connection.hpp"

# include <unistd.h> // write
#include <iostream>

extern int checkForbiddenHeaders(const std::vector<Cgi::Header>& headers);
extern bool isHeaderIgnored(const Cgi::Header& header);
extern BufferView getStatusMessage(Http::Status::Number statusCode);

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
		m_cgiRequest->setNotify_onError(NULL);
		mf_finishAndRelease();
		m_statusCode = Http::Status::BAD_GATEWAY;
		m_responseData->cgiPass = false;
		m_responseData->closeAfterSending = true;
		if (m_processHttpBody != &CgiResponse::mf_HttpBodyNone)
			m_processHttpBody = &CgiResponse::mf_HttpBodyNone;
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
		
		ASSERT_EQUAL(m_httpRequest != NULL, true, "CgiResponse::onCgiWrite(): no request assigned");
		if (m_httpRequest->forceParse() == Http::IOStatus::FINISHED)
			return (Cgi::IO::CLOSE);

		if (m_processHttpBody != &CgiResponse::mf_HttpBodyForward)
			return (Cgi::IO::CLOSE);
		return (Cgi::IO::CONTINUE);
	}

	Cgi::IO::State
	CgiResponse::onCgiReceiveHeaders(const Cgi::HeaderData& headers)
	{
		m_statusCode = (Http::Status::Number)headers.getStatusCode();
		m_headers = &headers;
		m_tempBody = headers.getTempBody();
		if (headers.getStatusCode() != Cgi::RequestConsts::Status::SUCCESS
		||	!checkForbiddenHeaders(headers.getHeaders()))
		{
			m_cgiRequest->setNotify_onError(NULL);
			mf_finishAndRelease();
			m_statusCode = Http::Status::BAD_GATEWAY;
			m_fillFunction = &CgiResponse::mf_fillErrorResponse;
			return (Cgi::IO::CLOSE);
		}
		m_fillFunction = &CgiResponse::mf_fillResponseLine;
		return (Cgi::IO::CONTINUE);
	}
}
