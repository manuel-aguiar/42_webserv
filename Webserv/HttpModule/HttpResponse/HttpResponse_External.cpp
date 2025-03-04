

#include "HttpResponse.hpp"
#include "../../ServerConfig/BlockFinder/BlockFinder.hpp"
#include "../../ServerContext/ServerContext.hpp"
#include "../../GenericUtils/Files/FilesUtils.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../GenericUtils/Buffer/Buffer.hpp"
#include "../HttpModule/HttpModule.hpp"
#include "../HttpCgiInterface/HttpCgiInterface.hpp"
#include "../../ServerConfig/ServerBlock/ServerBlock.hpp"

// Move to adequate scope
#define SERVER_NAME_VERSION "42_webserv/1.0"

// TODO: move to response scope
extern std::string DirectoryListing(const std::string& path);
extern std::string getMimeType(const std::string &path);

namespace Http
{
	void	Response::receiveRequestData(const Http::RequestData& data)
	{
		//std::cout << "receiving request data" << std::endl;
		m_responseData.headers.insert(std::make_pair("server", SERVER_NAME_VERSION));
		m_responseData.requestData = &data;
		m_responseData.requestStatus = data.status;

		m_fillFunction = &Response::mf_fillResponseLine;

		if (data.status == Http::Status::OK)
		{
			mf_validateHeaders();
		}

		//Full debug print of wtf is going on:
		// std::cout << "Request: " << m_responseData.requestData->method << " " << m_responseData.requestData->uri << " " << m_responseData.requestData->httpVersion << std::endl;
		// std::cout << "ServerBlock: " << m_responseData.serverBlock << std::endl;
		// std::cout << "Location: " << m_responseData.serverLocation << std::endl;
		// std::cout << "TargetPath: " << m_responseData.targetPath << std::endl;
		// std::cout << "TargetType: " << m_responseData.targetType << std::endl;
		// std::cout << "TargetExtension: " << m_responseData.targetExtension << std::endl;
		// std::cout << "ResponseType: " << m_responseData.responseType << std::endl;

		switch (m_responseData.responseType)
		{
			case ResponseData::CGI:
			{
				Http::CgiInterface& cgiInterface =
				reinterpret_cast<Http::Module*>(m_context.getAppLayerModule(Ws::AppLayer::HTTP))->accessCgiInterface();
				ASSERT_EQUAL(m_cgiResponse == NULL, true, "Response::receiveRequestData(): already had a gateway");
				
				m_cgiResponse = cgiInterface.acquireGateway();
				ASSERT_EQUAL(m_cgiResponse != NULL, true, "Response::receiveRequestData(): there must be a gateway available");
				
				if (!m_cgiResponse->initiateRequest(m_responseData))
				{
					m_responseData.requestStatus = Http::Status::SERVICE_UNAVAILABLE;
					mf_prepareErrorMessage();
					break ;
				}

				m_fillFunction = &Response::mf_fillCgiResponse;
				m_processFunction = &Response::mf_processBodyCgi;
				return ;
			}
			case ResponseData::STATIC:
				m_responseData.headers.insert(std::make_pair("content-length", StringUtils::to_string(m_file.size())));
				m_responseData.headers.insert(std::make_pair("content-type", getMimeType(m_responseData.targetPath)));

				m_fillFunctionBody = &Response::mf_sendStaticFile;
				break ;
			case ResponseData::REDIRECT:
				m_fillFunctionBody = &Response::mf_fillRedirect;
				break ;
			case ResponseData::FILE_UPLOAD:
				m_fillFunction = &Response::mf_fillNothingToSend;
				m_processFunction = &Response::mf_processBodyUpload;
				return ;
			case ResponseData::DIRECTORY_LISTING: // Directory Listing and Error have similar behavior
				m_defaultPageContent = DirectoryListing(m_responseData.targetPath);

				m_responseData.headers.insert(std::make_pair("content-length", StringUtils::to_string(m_defaultPageContent.size())));
				m_responseData.headers.insert(std::make_pair("content-type", "text/html"));

				m_fillFunctionBody = &Response::mf_fillDefaultPage;
				break ;
			case ResponseData::ERROR:
				mf_prepareErrorMessage();
				break ;
			case ResponseData::NO_CONTENT:
				m_fillFunctionBody = NULL;
				return ;
			default:
				return ;
		}

		// Common headers
		m_responseData.headers.insert(std::make_pair("server", SERVER_NAME_VERSION));
		m_responseData.headers.insert(std::make_pair("date", mf_getCurrentDate()));
		if (m_responseData.headers.find("content-length") == m_responseData.headers.end())
			m_responseData.headers.insert(std::make_pair("content-length", "0"));
		if (m_responseData.headers.find("content-type") == m_responseData.headers.end())
			m_responseData.headers.insert(std::make_pair("content-type", "plain/text"));
		if (m_responseData.closeAfterSending == true)
			m_responseData.headers.insert(std::make_pair("connection", "close"));
		else
			m_responseData.headers.insert(std::make_pair("connection", "keep-alive"));
		// ETag
		// Last-Modified

		m_currentHeader = m_responseData.headers.begin();

		// TEST CODE
		if (m_fillFunction == &Response::mf_fillNothingToSend)
		{
			if (m_responseData.requestStatus == Http::Status::OK)
				m_responseData.requestStatus = Http::Status::NOT_IMPLEMENTED;
			m_defaultPageContent = mf_generateDefaultErrorPage(m_responseData.requestStatus, "Implement Me (this is hardcoded)");
			m_fillFunction = &Response::mf_fillDefaultPage;
		}

		// Go to fillFunctions
	}

	BufferView
	Response::receiveRequestBody(const BufferView& view)
	{
		//std::cout << "Response received body, size: " << view.size() << std::endl;
		return ((this->*m_processFunction)(view));
	}


	Http::ResponseStatus::Type
	Response::fillWriteBuffer(BaseBuffer& writeBuffer)
	{
		return ((this->*m_fillFunction)(writeBuffer));
	}

	Http::ResponseStatus::Type
	Response::getStatus() const
	{
		return (m_status);
	}

	void
	Response::reset()
	{
		m_responseData.reset();
		m_fillFunction = &Response::mf_fillNothingToSend;
		m_fillFunctionBody = NULL;
		m_processFunction = &Response::mf_processBodyNone;
		m_pendingWrite.clear();
		m_status = ResponseStatus::WAITING;
		m_staticReadCounter = 0;
		m_file.reset();
		if (m_cgiResponse)
		{
			Http::CgiInterface& cgiInterface =
			reinterpret_cast<Http::Module*>(m_context.getAppLayerModule(Ws::AppLayer::HTTP))->accessCgiInterface();
			cgiInterface.releaseGateway(*m_cgiResponse);
			m_cgiResponse = NULL;
		}
	}

	void
	Response::close()
	{
		reset();
		m_connAddress = NULL;
	}	

	void
	Response::setConnectionAddress(const Ws::Sock::addr& addr)
	{
		m_connAddress = &addr;
	}

}
