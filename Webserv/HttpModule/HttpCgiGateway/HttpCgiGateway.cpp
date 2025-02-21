

# include "HttpCgiGateway.hpp"

namespace Http
{
	CgiGateway::CgiGateway(Cgi::Module& module, Http::Request& request, Http::Response& response)
		: m_module(module)
		, m_request(request)
		, m_response(response)
		, m_canRead(false)
		, m_canWrite(false)
		, m_readFd(Ws::FD_NONE)
		, m_writeFd(Ws::FD_NONE) {}

	CgiGateway::~CgiGateway() {}
	CgiGateway::CgiGateway(const CgiGateway& other)
		: m_module(other.m_module)
		, m_request(other.m_request)
		, m_response(other.m_response)
		, m_canRead(other.m_canRead)
		, m_canWrite(other.m_canWrite)
		, m_readFd(other.m_readFd)
		, m_writeFd(other.m_writeFd) {}

	CgiGateway&
	CgiGateway::operator=(const CgiGateway& other)
	{
		if (this == &other)
			return (*this);
		return (*this);
	}

	




	void
	CgiGateway::reset()
	{
		m_canRead = false;
		m_canWrite = false;
		m_readFd = Ws::FD_NONE;
		m_writeFd = Ws::FD_NONE;
	}
}