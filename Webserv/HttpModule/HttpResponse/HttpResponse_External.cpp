

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
		m_responseData.requestData = &data;
		m_responseData.requestStatus = data.status;

		m_fillFunction = &Response::mf_fillResponseLine;

		if (data.status == Http::Status::OK)
		{
			mf_validateHeaders();
		}

		// Full debug print of wtf is going on:
		// std::cout << "Request: " << m_responseData.requestData->method << " " << m_responseData.requestData->uri << " " << m_responseData.requestData->httpVersion << std::endl;
		// std::cout << "ServerBlock: " << m_responseData.serverBlock << std::endl;
		// std::cout << "Location: " << m_responseData.serverLocation << std::endl;
		// std::cout << "TargetPath: " << m_responseData.targetPath << std::endl;
		// std::cout << "TargetType: " << m_responseData.targetType << std::endl;
		// std::cout << "TargetExtension: " << m_responseData.targetExtension << std::endl;
		// std::cout << "ResponseType: " << m_responseData.responseType << std::endl;

		mf_addHeader("server", SERVER_NAME_VERSION);
		mf_addHeader("date", mf_getCurrentDate());
		mf_addHeader("connection", (m_responseData.closeAfterSending ? "close" : "keep-alive"));

		switch (m_responseData.responseType)
		{
			case ResponseData::CGI:
			{
				Http::CgiInterface& cgiInterface =
				reinterpret_cast<Http::Module*>(m_context.getAppLayerModule(Ws::AppLayer::HTTP))->accessCgiInterface();
				m_cgiResponse = cgiInterface.acquireGateway();

				ASSERT_EQUAL(m_cgiResponse != NULL, true, "Response::receiveRequestData(): failed to acquire cgi gateway");
				m_cgiResponse->initiateRequest(m_responseData);
				m_fillFunction = &Response::mf_fillCgiResponse;
				m_processFunction = &Response::mf_processBodyCgi;
				return ;
			}
			case ResponseData::STATIC:
				mf_addContentHeaders(m_file.size(), getMimeType(m_responseData.targetPath));

				if (mf_addCacheControlHeaders())
					m_fillFunctionBody = NULL;
				else
					m_fillFunctionBody = &Response::mf_sendStaticFile;
				break ;
			case ResponseData::REDIRECT:
				m_fillFunctionBody = &Response::mf_fillRedirect;
				break ;
			case ResponseData::FILE_UPLOAD:
				m_fillFunction = &Response::mf_fillNothingToSend;
				m_processFunction = &Response::mf_processBodyUpload;
				break ;
			case ResponseData::DIRECTORY_LISTING: // Directory Listing and Error have similar behavior
				m_defaultPageContent = DirectoryListing(m_responseData.targetPath);

				mf_addContentHeaders(m_defaultPageContent.size(), "text/html");

				m_fillFunctionBody = &Response::mf_fillDefaultPage;
				break ;
			case ResponseData::ERROR:
				if (m_responseData.serverBlock != NULL)
				{
					if (m_responseData.serverBlock->getErrorPages().find(m_responseData.requestStatus) != m_responseData.serverBlock->getErrorPages().end())
					{
						mf_prepareStaticFile(m_responseData.serverBlock->getErrorPages().find(m_responseData.requestStatus)->second.c_str());
						mf_addContentHeaders(m_file.size(), getMimeType(m_responseData.serverBlock->getErrorPages().find(m_responseData.requestStatus)->second.c_str()));

						m_fillFunctionBody = &Response::mf_sendStaticFile;
						break ;
					}
				}
				mf_addContentHeaders(m_defaultPageContent.size(), "text/html");
				m_defaultPageContent = mf_generateDefaultErrorPage(m_responseData.requestStatus, "Task failed successfully");
				m_fillFunctionBody = &Response::mf_fillDefaultPage;

				break ;
			case ResponseData::NO_CONTENT:
				mf_addHeader("content-length", "0");
				m_fillFunctionBody = NULL;
				break ;
			default:
				break ;
		}

		m_currentHeader = m_responseData.headers.begin();

		return ;
		
		
		// mf_addHeader("content-length", "0");
		// mf_addHeader("content-type", "plain/text");


		// // TEST CODE
		// if (m_fillFunction == &Response::mf_fillNothingToSend)
		// {
		// 	if (m_responseData.requestStatus == Http::Status::OK)
		// 		m_responseData.requestStatus = Http::Status::NOT_IMPLEMENTED;
		// 	m_defaultPageContent = mf_generateDefaultErrorPage(m_responseData.requestStatus, "Implement Me (this is hardcoded)");
		// 	m_fillFunction = &Response::mf_fillDefaultPage;
		// }

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
		}
		m_cgiResponse = NULL;
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
