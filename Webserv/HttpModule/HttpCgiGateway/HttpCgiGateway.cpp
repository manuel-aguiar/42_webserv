

# include "HttpCgiGateway.hpp"
# include "../HttpRequest/HttpRequest.hpp"
# include "../HttpResponse/HttpResponse.hpp"

# include "../../GenericUtils/Buffer/Buffer.hpp"

# include <unistd.h> // read/write

namespace Http
{
HttpCgiGateway::HttpCgiGateway(Cgi::Module& cgi, Http::Response& response) : 
	m_module(cgi),
	m_cgiRequest(NULL),
	m_response(response),
	m_request(response.getRequest()),
	m_connection(m_request.getConnection()),
	m_state(NONE),
	m_readFd(Ws::FD_NONE),
	m_readAvailable(false),
	m_msgBodyOffset(0)
{
}

HttpCgiGateway::~HttpCgiGateway()
{
}

void	HttpCgiGateway::close()
{
	if (m_cgiRequest)
	{
		m_module.finishRequest(*m_cgiRequest, true);
		m_cgiRequest = NULL;
	}
}

void	HttpCgiGateway::prepareRequest()
{
	m_cgiRequest = m_module.acquireRequest();
	if (!m_cgiRequest)
	{
		// inform response, no can do
		return ;
	}

	// fill request

}

void
HttpCgiGateway::onSuccess					(Cgi::User user)
{
	// inform response, all done
}

void
HttpCgiGateway::onErrorStartup				(Cgi::User user)
{
	// inform response, no can do
}

void
HttpCgiGateway::onErrorRuntime				(Cgi::User user)
{
	// inform response, no can do
}

void
HttpCgiGateway::onErrorTimeOut				(Cgi::User user)
{
	// inform response, no can do
}

Cgi::IO::BytesCount
HttpCgiGateway::onRead	(Cgi::User user, Ws::fd readFd)
{
	Http::HttpCgiGateway* gateway = reinterpret_cast<Http::HttpCgiGateway*>(user);
	gateway->storeReadAvailable(readFd);
	return (1);
}

Cgi::IO::BytesCount
HttpCgiGateway::onWrite	(Cgi::User user, Ws::fd writeFd)
{
	Http::HttpCgiGateway* gateway = reinterpret_cast<Http::HttpCgiGateway*>(user);
	return (gateway->write(writeFd));
}


void
HttpCgiGateway::storeReadAvailable(const Ws::fd& readFd)
{
	m_readFd = readFd;
	m_readAvailable = true;
}

/*

	Read is controlled by client availability. 
	When client is available, read is called.
	When read is called, we read from the fd and store the data in the request.
	When we have read all the data, we disable read.

*/
void
HttpCgiGateway::read()
{
	Buffer buffer;

	if (!m_readAvailable)
		return ;
	
	buffer.read(m_readFd);

	switch (m_state)
	{
		case NONE:
		{
			// parse headers
			m_state = HEADERS;
			break ;
		}
	}	

	m_readAvailable = false;
}

Cgi::IO::BytesCount
HttpCgiGateway::write(Ws::fd writeFd)
{
	const std::string& msgBody = m_request.getMsgBody();

	if (m_msgBodyOffset >= msgBody.size())
		return (0); // nothing left to write to script.
	
	int bytesWritten = ::write(writeFd, msgBody.c_str() + m_msgBodyOffset, msgBody.size() - m_msgBodyOffset);

	m_msgBodyOffset += bytesWritten;
	
	// if everything is written now, disable write
	return (m_msgBodyOffset == msgBody.size() ? 0 : 1);
}

} // namespace Http