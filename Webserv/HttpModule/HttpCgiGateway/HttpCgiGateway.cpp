

# include "HttpCgiGateway.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"

extern int validateHeaders(const std::vector<Cgi::Header>& headers);

namespace Http
{
	CgiGateway::CgiGateway(Cgi::Module& module, Http::Request& request, Http::Response& response)
		: m_module(module)
		, m_request(request)
		, m_response(response)
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

		}

		void
		CgiGateway::onError()
		{

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
			return (Cgi::IO::State::CONTINUE);
		}

	void
	CgiGateway::reset()
	{
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
		}
	}



	Response::Status
	CgiGateway::mf_bodyStream(BaseBuffer& writeBuffer)
	{
		return (Response::Status::WAITING);
	}














	CgiGateway::~CgiGateway() {}
	CgiGateway::CgiGateway(const CgiGateway& other)
		: m_module(other.m_module)
		, m_request(other.m_request)
		, m_response(other.m_response)
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