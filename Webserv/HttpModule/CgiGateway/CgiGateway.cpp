

# include "CgiGateway.hpp"
# include "../Request/Request.hpp"
# include "../Response/Response.hpp"

namespace Http
{
CgiGateway::CgiGateway(Cgi::Module& cgi, Http::Response& response) : 
	m_module(cgi),
	m_cgiRequest(NULL),
	m_response(response),
	m_request(response.getRequest()),
	m_connection(m_request.getConnection())
{
}

CgiGateway::~CgiGateway()
{
}

void	CgiGateway::close()
{
	if (m_cgiRequest)
	{
		m_module.finishRequest(*m_cgiRequest, true);
		m_cgiRequest = NULL;
	}
}

void	CgiGateway::prepareRequest()
{
	m_cgiRequest = m_module.acquireRequest();
	if (!m_cgiRequest)
	{
		// inform response, no can do
	}
}

void
CgiGateway::onSuccess					(Cgi::User user)
{
	// inform response, all done
}

void
CgiGateway::onErrorStartup				(Cgi::User user)
{
	// inform response, no can do
}

void
CgiGateway::onErrorRuntime				(Cgi::User user)
{
	// inform response, no can do
}

void
CgiGateway::onErrorTimeOut				(Cgi::User user)
{
	// inform response, no can do
}

Cgi::IO::BytesCount
CgiGateway::onRead	(Cgi::User user, int readFd)
{

}

Cgi::IO::BytesCount
CgiGateway::onWrite	(Cgi::User user, int writeFd)
{

}



} // namespace Http