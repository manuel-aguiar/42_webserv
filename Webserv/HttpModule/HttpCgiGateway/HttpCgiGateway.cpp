

# include "HttpCgiGateway.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"

extern int checkForbiddenHeaders(const std::vector<Cgi::Header>& headers);

extern const char* getStatusMessage(int statusCode);

namespace Http
{
	CgiGateway::CgiGateway(Cgi::Module& module, Http::Request& request, Http::Response& response)
		: m_module(module)
		, m_cgiRequest(NULL)
		, m_httpRequest(request)
		, m_httpResponse(response)
		, m_canRead(false)
		, m_canWrite(false)
		, m_readFd(Ws::FD_NONE)
		, m_writeFd(Ws::FD_NONE) 
		, m_statusCode(-1)
		, m_headers(NULL)
		, m_responseState(IDLE) {}


		void
		CgiGateway::onSuccess()
		{
			// dunno yet
		}

		void
		CgiGateway::onError()
		{
			m_statusCode = Http::Status::INTERNAL_ERROR;
			m_responseState = ERROR;
		}

		Cgi::IO::State
		CgiGateway::onRead(const Ws::fd readFd)
		{
			m_readFd = readFd;
			m_canRead = true;
			return (Cgi::IO::State::CONTINUE);
		}

		Cgi::IO::State
		CgiGateway::onWrite(const Ws::fd writeFd)
		{
			m_writeFd = writeFd;
			m_canWrite = true;
			return (Cgi::IO::State::CONTINUE);
		}

		Cgi::IO::State
		CgiGateway::onReceiveHeaders(Cgi::HeaderData& headers)
		{
			m_statusCode = headers.getStatusCode();
			m_headers = &headers;
			if (!checkForbiddenHeaders(headers.getHeaders()))
			{
				m_cgiRequest->setNotify_onError(NULL);	//disable error notification from premature closure
				m_module.finishRequest(*m_cgiRequest, true);
				m_statusCode = Http::Status::BAD_GATEWAY;
				m_responseState = ERROR;
			}
			return (Cgi::IO::State::CLOSE);
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
		m_responseState = IDLE;
	}




	Response::Status
	CgiGateway::fillWriteBuffer(BaseBuffer& writeBuffer)
	{
		switch (m_responseState)
		{
			case IDLE:
				return (Response::Status::WAITING);
			case PROCESSING:
				return (Response::Status::WAITING);
			case RESPONSE_LINE:
				return (Response::Status::WAITING);
			case HEADERS:
				return (Response::Status::WAITING);
			case BODY_TEMP:
				return (Response::Status::WAITING);
			case BODY_STREAM:
				return (Response::Status::WAITING);
			case COMPLETE:
				return (Response::Status::WAITING);
			case ERROR:
				return (mf_fillErrorResponse(writeBuffer));
		}
	}

	Response::Status
	CgiGateway::mf_fillErrorResponse(BaseBuffer& writeBuffer)
	{
		writeBuffer.push("HTTP/1.1 ", 9);
		writeBuffer.push(std::to_string(m_statusCode).c_str(), std::to_string(m_statusCode).size());
		writeBuffer.push(" ", 1);
		writeBuffer.push(getStatusMessage(m_statusCode));
		writeBuffer.push("\r\n", 2);
		writeBuffer.push("Content-Length: 37\r\n", 19);
		writeBuffer.push("Connection: close\r\n", 19);
		writeBuffer.push("\r\n", 2);
		writeBuffer.push("Cgi script failed to execute correctly", 37);

		return (Response::Status::FINISHED);
	}

	Response::Status
	CgiGateway::mf_bodyStream(BaseBuffer& writeBuffer)
	{
		return (Response::Status::WAITING);
	}














	CgiGateway::~CgiGateway() {}
	CgiGateway::CgiGateway(const CgiGateway& other)
		: m_module(other.m_module)
		, m_httpRequest(other.m_httpRequest)
		, m_httpResponse(other.m_httpResponse)
		, m_canRead(other.m_canRead)
		, m_canWrite(other.m_canWrite)
		, m_readFd(other.m_readFd)
		, m_writeFd(other.m_writeFd)
		, m_statusCode(other.m_statusCode)
		, m_headers(other.m_headers)
		, m_responseState(other.m_responseState) {}

	CgiGateway&
	CgiGateway::operator=(const CgiGateway& other)
	{
		if (this == &other)
			return (*this);
		return (*this);
	}	
}