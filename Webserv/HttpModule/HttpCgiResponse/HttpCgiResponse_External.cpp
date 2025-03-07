

# include "HttpCgiResponse.hpp"
# include "CgiHandlers.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../ServerConfig/ServerLocation/ServerLocation.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"
# include "../../Connections/Connection/Connection.hpp"


# include <arpa/inet.h>

static const char* contentLengthFind = "Content-Length";
static const char* contentTypeFind = "Content-Type";


#ifndef NDEBUG
	static int testHeadersOfInterest();
	static const int g_testHeadersOfInterest = testHeadersOfInterest();
	static int testHeadersOfInterest()
	{
		std::string test = contentLengthFind;
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToCapitalized() == BufferView(contentLengthFind), true, "contentLengthFind is not correctly formated");
		test = contentTypeFind;
		ASSERT_EQUAL(BufferView(test).trim(" \r\n\t\v").modify_ToCapitalized() == BufferView(contentTypeFind), true, "contentTypeFind is not correctly formated");
		return (0);
	}
	
#endif

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
		return ((this->*m_processHttpBody)(view));
	}


	Http::IOStatus::Type
	CgiResponse::fillWriteBuffer(BaseBuffer& writeBuffer)
	{
		if (m_processHttpBody != &CgiResponse::mf_HttpBodyNone)
			return (Http::IOStatus::WAITING);
		return ((this->*m_fillFunction)(writeBuffer));
	}

	bool
	CgiResponse::initiateRequest(Http::ResponseData& responseData, Http::Request& request, const Conn::Connection* connection)
	{
		m_responseData = &responseData;
		m_httpRequest = &request;

		const Http::RequestData& data = *responseData.requestData;

		Http::RequestData::headerContainer::const_iterator finder;

        const std::map<std::string, std::string>& interpreterMap = responseData.serverLocation->getCgiInterpreters();
        std::map<std::string, std::string>::const_iterator interpPtr = interpreterMap.find(responseData.targetExtension);

        if (interpPtr == interpreterMap.end())
            return (false);

		ASSERT_EQUAL(m_cgiRequest == NULL, true, "CgiResponse::initiateRequest(): already had a request");

		m_cgiRequest = m_module.acquireRequest();
		
		if (!m_cgiRequest)
			return (false);
		
        m_cgiRequest->setTimeoutMs(5000);

		m_cgiRequest->setUser(this);
		m_cgiRequest->setNotify_onSuccess(&CgiHandlers::onSuccess);
		m_cgiRequest->setNotify_onError(&CgiHandlers::onError);
		m_cgiRequest->setWriteToScript_Callback(&CgiHandlers::onWrite);
		m_cgiRequest->setReadBodyFromScript_Callback(&CgiHandlers::onRead);
		m_cgiRequest->setReceiveStatusHeaders_Callback(&CgiHandlers::onReceiveHeaders);
		
		m_cgiRequest->setInterpreterPath(interpPtr->second);

		// CONTENT-LENGTH
		finder = data.headers.find(contentLengthFind);
		if (finder != data.headers.end())
			m_cgiRequest->setEnvBase(Cgi::Env::Enum::CONTENT_LENGTH, finder->second);

		// CONTENT-TYPE
		finder = data.headers.find(contentTypeFind);
		if (finder != data.headers.end())
			m_cgiRequest->setEnvBase(Cgi::Env::Enum::CONTENT_TYPE, finder->second);

		// GATEWAY_INTERFACE CGI/1.1
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::GATEWAY_INTERFACE, "CGI/1.1");
		
        m_cgiRequest->setScriptPath(responseData.targetPath);

		if (connection)
		{
			// REMOTE_ADDR
			char ipPeer[INET_ADDRSTRLEN];
			::inet_ntop(AF_INET, &connection->info_getPeerInfo().addr.sockaddr_in.sin_addr, ipPeer, INET_ADDRSTRLEN);
			m_cgiRequest->setEnvBase(Cgi::Env::Enum::REMOTE_ADDR, 
				StringUtils::to_string(ipPeer));
			// SERVER_PORT -> from connection
			m_cgiRequest->setEnvBase(Cgi::Env::Enum::SERVER_PORT, 
				StringUtils::to_string(::ntohs(connection->info_getListenInfo().addr.sockaddr_in.sin_port)));
		}

		// PATH_INFO
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::PATH_INFO, data.path);

		// QUERY_STRING
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::QUERY_STRING, data.queryString);
		
		// REDIRECT_STATUS
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::REDIRECT_STATUS, StringUtils::to_string(data.status));

		// REQUEST_METHOD -> HttpRequestData
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::REQUEST_METHOD, data.method);
		
		// SCRIPT_NAME	-> script
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::SCRIPT_NAME, responseData.targetPath);
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::SCRIPT_FILENAME, responseData.targetPath);

		// SERVER_NAME -> webserv
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::SERVER_NAME, "42_webserv");


		// SERVER_PROTOCOL HTTP/1.1
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::SERVER_PROTOCOL, "HTTP/1.1");
		
		// SERVER_SOFTWARE Webserv
		m_cgiRequest->setEnvBase(Cgi::Env::Enum::SERVER_SOFTWARE, "42_webserv");
		
		m_module.enqueueRequest(*m_cgiRequest, true);

        m_processHttpBody = &CgiResponse::mf_HttpBodySend;

		return (true);
	}

}
