

# include "HttpCgiResponse.hpp"
# include "CgiHandlers.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../ServerConfig/ServerLocation/ServerLocation.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"
# include "../../Connections/Connection/Connection.hpp"

# include <unistd.h> // write

#include <iostream>

namespace Http
{

	// must send everything before starting to write to client
	// therefore, try to send request body full until receiving an empty view (EOF)
	//           -> engage the fill function pointers
	// if script close the write pipe, turn into send body ignore
	// ignore any data that comes in, once receiving an EOF, engage the
	//           -> engage the fill function pointers

	BufferView
	CgiResponse::sendHttpBody(const BufferView& view)
	{
		std::cout << "send body, size: " << view.size() << std::endl;
		return ((this->*m_processHttpBody)(view));
	}


	Http::ResponseStatus::Type
	CgiResponse::fillWriteBuffer(BaseBuffer& writeBuffer)
	{
		//std::cout << "cgi fillWriteBuffer" << std::endl;
		// still processing body, can't start writing the response yet
		if (m_processHttpBody != &CgiResponse::mf_HttpBodyNone)
		{
			//std::cout << "receiving body, waiting" << std::endl;
			return (Http::ResponseStatus::WAITING);
		}
		//std::cout << "can start writing" << std::endl;
		//std::cout << "can start writing" << std::endl;
		return ((this->*m_fillFunction)(writeBuffer));
	}

	bool
	CgiResponse::initiateRequest(const Http::ResponseData& responseData)
	{
		const Http::RequestData& data = *responseData.requestData;

		Http::RequestData::headerContainer::const_iterator finder;

        const std::map<std::string, std::string>& interpreterMap = responseData.serverLocation->getCgiInterpreters();
        std::map<std::string, std::string>::const_iterator interpPtr = interpreterMap.find(responseData.targetExtension);

		m_cgiRequest = m_module.acquireRequest();

		if (!m_cgiRequest)
			return (false);

		m_cgiRequest = m_module.acquireRequest();

		if (!m_cgiRequest)
			return (false);

        if (interpPtr == interpreterMap.end())
        {
            m_cgiRequest->setNotify_onError(NULL);	//disable error notification from premature closure
            m_statusCode = Http::Status::BAD_GATEWAY;
            m_fillFunction = &CgiResponse::mf_fillErrorResponse;
            return (false);
        }

		
        m_cgiRequest->setTimeoutMs(10000);

		m_cgiRequest->setUser(this);
		m_cgiRequest->setNotify_onSuccess(&CgiHandlers::onSuccess);
		m_cgiRequest->setNotify_onError(&CgiHandlers::onError);
		m_cgiRequest->setWriteToScript_Callback(&CgiHandlers::onWrite);
		m_cgiRequest->setReadBodyFromScript_Callback(&CgiHandlers::onRead);
		m_cgiRequest->setReceiveStatusHeaders_Callback(&CgiHandlers::onReceiveHeaders);

		// looking at location to find the interpreter
		// m_cgiRequest->setInterpreterPath(interpreterPath); // hardcoded for now


		// CONTENT-LENGTH
		finder = data.headers.find("content-length");
		if (finder != data.headers.end())
			m_cgiRequest->setEnvBase(Cgi::Env::Enum::CONTENT_LENGTH, finder->second);

		// CONTENT-TYPE
		finder = data.headers.find("content-type");
		if (finder != data.headers.end())
			m_cgiRequest->setEnvBase(Cgi::Env::Enum::CONTENT_TYPE, finder->second);

		// GATEWAY_INTERFACE CGI/1.1
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::GATEWAY_INTERFACE, "CGI/1.1");
		
        m_cgiRequest->setInterpreterPath(interpPtr->second);

        m_cgiRequest->setScriptPath(responseData.targetPath);

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
		
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::REDIRECT_STATUS, StringUtils::to_string(data.status));

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

        m_processHttpBody = &CgiResponse::mf_HttpBodySend;

		return (true);
	}

}