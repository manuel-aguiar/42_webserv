

# include "HttpCgiGateway.hpp"
# include "CgiHandlers.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"
# include "../../Connections/Connection/Connection.hpp"

# include <unistd.h> // write


namespace Http
{

	// must send everything before starting to write to client
	// therefore, try to send request body full until receiving an empty view (EOF)
	//           -> engage the fill function pointers
	// if script close the write pipe, turn into send body ignore
	// ignore any data that comes in, once receiving an EOF, engage the
	//           -> engage the fill function pointers

	BufferView
	CgiGateway::sendHttpBody(const BufferView& view)
	{
		return ((this->*m_processHttpBody)(view));
	}


	Http::ResponseStatus::Type
	CgiGateway::fillWriteBuffer(BaseBuffer& writeBuffer)
	{
		// still processing body, can't start writing the response yet
		if (m_processHttpBody != &CgiGateway::mf_HttpBodyNone)
			return (Http::ResponseStatus::WAITING);
		return ((this->*m_fillFunction)(writeBuffer));
	}

	bool
	CgiGateway::initiateRequest(const Http::ResponseData& responseData)
	{
		const Http::RequestData& data = *responseData.requestData;

		Http::RequestData::headerContainer::const_iterator finder;

		m_cgiRequest = m_module.acquireRequest();

		if (!m_cgiRequest)
			return (false);
		
		m_cgiRequest->setUser(this);
		m_cgiRequest->setNotify_onSuccess(&CgiHandlers::onSuccess);
		m_cgiRequest->setNotify_onError(&CgiHandlers::onError);
		m_cgiRequest->setWriteToScript_Callback(&CgiHandlers::onWrite);
		m_cgiRequest->setReadBodyFromScript_Callback(&CgiHandlers::onRead);
		m_cgiRequest->setReceiveStatusHeaders_Callback(&CgiHandlers::onReceiveHeaders);

		// looking at location to find the interpreter
		// m_cgiRequest->setInterpreterPath(interpreterPath); // hardcoded for now


		// CONTENT-LENGTH
		finder = data.headers.find("Content-Length");
		if (finder != data.headers.end())
			m_cgiRequest->setEnvBase(Cgi::Env::Enum::CONTENT_LENGTH, finder->second);

		// CONTENT-TYPE
		finder = data.headers.find("Content-Type");
		if (finder != data.headers.end())
			m_cgiRequest->setEnvBase(Cgi::Env::Enum::CONTENT_TYPE, finder->second);

		// GATEWAY_INTERFACE CGI/1.1
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::GATEWAY_INTERFACE, "CGI/1.1");
		
		// REMOTE_ADDR -> from the connection
		//if (connection)
		//{
		//	m_cgiRequest->setEnvBase(Cgi::Env::Enum::SERVER_PORT, 
		//		StringUtils::to_string(::ntohl(connection->info_getPeerInfo().addr.sockaddr_in.sin_addr.s_addr)));
		//}

		// PATH_INFO
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::PATH_INFO, data.path);

		// PATH_TRANSLATED

		
		// QUERY_STRING
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::QUERY_STRING, data.queryString);
		
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::REDIRECT_STATUS, "200");

		// REQUEST_METHOD -> HttpRequestData
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::REQUEST_METHOD, data.method);
		
		// SCRIPT_NAME	-> script
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::SCRIPT_NAME, responseData.targetPath);
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::SCRIPT_FILENAME, responseData.targetPath);

		// SERVER_NAME -> webserv
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::SERVER_NAME, "42_webserv");

		// SERVER_PORT -> from connection
		//if (connection)
		//{
		//	m_cgiRequest->setEnvBase(Cgi::Env::Enum::SERVER_PORT, 
		//		StringUtils::to_string(::ntohs(connection->info_getPeerInfo().addr.sockaddr_in.sin_port)));
		//}

		// SERVER_PROTOCOL HTTP/1.1
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::SERVER_PROTOCOL, "HTTP/1.1");
		
		// SERVER_SOFTWARE Webserv
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::SERVER_SOFTWARE, "42_webserv");
		
		m_module.enqueueRequest(*m_cgiRequest, true);

		return (true);
	}

}