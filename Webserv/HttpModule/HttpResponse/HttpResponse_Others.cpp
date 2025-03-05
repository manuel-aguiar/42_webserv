


#include "HttpResponse.hpp"
#include "../HttpModule/HttpModule.hpp"
#include "../HttpCgiInterface/HttpCgiInterface.hpp"
#include "../../GenericUtils/Files/FilesUtils.hpp"
#include "../../ServerConfig/BlockFinder/BlockFinder.hpp"
#include "../../ServerContext/ServerContext.hpp"
#include "../../ServerConfig/ServerBlock/ServerBlock.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../GenericUtils/Buffer/Buffer.hpp"

extern const char* getStatusMessage(int statusCode);
extern std::string getMimeType(const std::string &path);
# define SERVER_NAME_VERSION "webserv/1.0"

namespace Http
{
	ResponseData		Response::getResponseData() const
	{
		return (m_responseData);
	}

	std::string			Response::mf_generateRedirectPage(int statusCode, const std::string& location)
	{
		std::stringstream ss;
		const char* statusText = getStatusMessage(statusCode);

		ss << "<!DOCTYPE html>\n"
		<< "<html>\n"
		<< "<head>\n"
		<< "<title>" << statusCode << " " << statusText << "</title>\n"
		<< "<style>\n"
		<< "body {\n"
		<< "    font-family: system-ui, sans-serif;\n"
		<< "    margin: 40px auto;\n"
		<< "    max-width: 1750px;\n"
		<< "    padding: 0 10px;\n"
		<< "    color: #444;\n"
		<< "    text-align: center;\n"
		<< "}\n"
		<< ".server {\n"
		<< "    font-size: 20px;\n"
		<< "    font-weight: bold;\n"
		<< "}\n"
		<< ".status-code {\n"
		<< "    font-size: 48px;\n"
		<< "    font-weight: bold;\n"
		<< "    color:rgba(0, 128, 255, 0.91);\n"
		<< "    margin: 30px 0 10px 0;\n"
		<< "}\n"
		<< "h1 {\n"
		<< "    margin: 10px 0;\n"
		<< "}\n"
		<< "hr {\n"
		<< "    width: 100%;\n"
		<< "    margin: 10px auto;\n"
		<< "    border: none;\n"
		<< "    border-top: 1px solid #ddd;\n"
		<< "}\n"
		<< "</style>\n"
		<< "</head>\n"
		<< "<body>\n"
		<< "    <div class=\"server\">" << SERVER_NAME_VERSION << "</div>\n"
		<< "    <hr>\n"
		<< "    <div class=\"status-code\">" << statusCode << "</div>\n"
		<< "    <h1>" << statusText << "</h1>\n"
		<< "    <p>The request resource has been " << statusText << " <a href=\"" << location << "\">here</a></p>\n"
		<< "</body>\n"
		<< "</html>\n";
		return (ss.str());
	}

	std::string Http::Response::mf_generateDefaultErrorPage(int statusCode, const std::string& errorMessage)
	{
		std::stringstream ss;
		const char* statusText = getStatusMessage(statusCode);

		ss << "<!DOCTYPE html>\n"
		<< "<html>\n"
		<< "<head>\n"
		<< "<title>" << statusCode << " " << statusText << "</title>\n"
		<< "<style>\n"
		<< "body {\n"
		<< "    font-family: system-ui, sans-serif;\n"
		<< "    margin: 40px auto;\n"
		<< "    max-width: 1750px;\n"
		<< "    padding: 0 10px;\n"
		<< "    color: #444;\n"
		<< "    text-align: center;\n"
		<< "}\n"
		<< ".server {\n"
		<< "    font-size: 20px;\n"
		<< "    font-weight: bold;\n"
		<< "}\n"
		<< ".status-code {\n"
		<< "    font-size: 48px;\n"
		<< "    font-weight: bold;\n"
		<< "    color: #e74c3c;\n"
		<< "    margin: 30px 0 10px 0;\n"
		<< "}\n"
		<< "h1 {\n"
		<< "    margin: 10px 0;\n"
		<< "}\n"
		<< "hr {\n"
		<< "    width: 100%;\n"
		<< "    margin: 10px auto;\n"
		<< "    border: none;\n"
		<< "    border-top: 1px solid #ddd;\n"
		<< "}\n"
		<< "</style>\n"
		<< "</head>\n"
		<< "<body>\n"
		<< "    <div class=\"server\">" << SERVER_NAME_VERSION << "</div>\n"
		<< "    <hr>\n"
		<< "    <div class=\"status-code\">" << statusCode << "</div>\n"
		<< "    <h1>" << statusText << "</h1>\n"
		<< "    <p>" << errorMessage << "</p>\n"
		<< "</body>\n"
		<< "</html>\n";

		return (ss.str());
	}


	void
	Response::mf_prepareErrorMessage()
	{
		if (m_responseData.serverBlock != NULL)
		{
			if (m_responseData.serverBlock->getErrorPages().find(m_responseData.requestStatus) != m_responseData.serverBlock->getErrorPages().end())
			{
				mf_prepareStaticFile(m_responseData.serverBlock->getErrorPages().find(m_responseData.requestStatus)->second.c_str());
				mf_addContentHeaders(m_file.size(), getMimeType(m_responseData.serverBlock->getErrorPages().find(m_responseData.requestStatus)->second.c_str()));

				m_fillFunctionBody = &Response::mf_sendStaticFile;
				return ;
			}
		}
		m_defaultPageContent = mf_generateDefaultErrorPage(m_responseData.requestStatus, m_responseData.errorMessage);
		mf_addContentHeaders(m_defaultPageContent.size(), "text/html");
		m_fillFunctionBody = &Response::mf_fillDefaultPage;
	}

	void
	Response::mf_prepareCgiExecution()
	{
		Http::CgiInterface& cgiInterface =
		reinterpret_cast<Http::Module*>(m_context.getAppLayerModule(Ws::AppLayer::HTTP))->accessCgiInterface();
		ASSERT_EQUAL(m_cgiResponse == NULL, true, "Response::receiveRequestData(): already had a gateway");
		
		m_cgiResponse = cgiInterface.acquireGateway();
		ASSERT_EQUAL(m_cgiResponse != NULL, true, "Response::receiveRequestData(): there must be a gateway available");
		
		if (!m_cgiResponse->initiateRequest(m_responseData, m_tcpConn))
		{
			m_responseData.requestStatus = Http::Status::SERVICE_UNAVAILABLE;
			mf_prepareErrorMessage();
			return ;
		}

		m_fillFunction = &Response::mf_fillCgiResponse;
		m_processFunction = &Response::mf_processBodyCgi;
	}

} // namespace Http
