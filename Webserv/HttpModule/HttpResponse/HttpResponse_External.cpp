

#include "HttpResponse.hpp"
#include "../../ServerConfig/BlockFinder/BlockFinder.hpp"
#include "../../ServerContext/ServerContext.hpp"
#include "../../GenericUtils/Files/FilesUtils.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../GenericUtils/Buffer/Buffer.hpp"
#include "../HttpModule/HttpModule.hpp"
#include "../HttpCgiInterface/HttpCgiInterface.hpp"

// Move to adequate scope
#define SERVER_NAME_VERSION "42_webserv/1.0"

// TODO: move to response scope
extern std::string DirectoryListing(const std::string& path);

# include <cstdlib>

namespace Http
{
	void	Response::receiveRequestData(const Http::RequestData& data)
	{
		m_responseData.headers.insert(std::make_pair("server", SERVER_NAME_VERSION));
		m_responseData.requestData = &data;
		m_responseData.requestStatus = data.status;

		m_fillFunction = &Response::mf_fillResponseLine;

		if (data.status == Http::Status::OK)
		{

			mf_validateHeaders();

			// DELETE FILE
		}

		size_t contentLength = 0;
		std::string contentType = "";

		switch (m_responseData.responseType)
		{
			case ResponseData::CGI:
				// m_processFunction = &Response::mf_processBodyCgi;
				// m_fillFunction = &Response::mf_fillCgiResponse;
				return ;
			case ResponseData::STATIC:
				// mf_prepareStaticFile();
				// m_fillFunctionBody = &Response::mf_fillStaticFile;
				break ;
			case ResponseData::REDIRECT:
				m_fillFunctionBody = &Response::mf_fillRedirect;
				break ;
			case ResponseData::DIRECTORY_LISTING:
				DirectoryListing(m_responseData.targetPath);
				m_fillFunctionBody = &Response::mf_fillDirectoryListing;
				break ;
			case ResponseData::FILE_UPLOAD:
				m_fillFunction = &Response::mf_fillNothingToSend;
				m_processFunction = &Response::mf_processBodyUpload;
				return ;
			case ResponseData::ERROR:
				/* fall through */
			default:
				m_fillFunctionBody = &Response::mf_fillErrorResponse;
				return ;
		}

		// Common headers
		m_responseData.headers.insert(std::make_pair("content-length", StringUtils::to_string(contentLength)));
		m_responseData.headers.insert(std::make_pair("content-type", contentType));


		m_responseData.headers.insert(std::make_pair("server", SERVER_NAME_VERSION));
		m_responseData.headers.insert(std::make_pair("date", mf_getCurrentDate()));
		if (m_responseData.closeAfterSending == true)
			m_responseData.headers.insert(std::make_pair("connection", "close"));
		else
			m_responseData.headers.insert(std::make_pair("connection", "keep-alive"));
		// ETag
		// Last-Modified


		if (m_fillFunction == &Response::mf_fillNothingToSend)
		{
			if (m_responseData.requestStatus == Http::Status::OK)
				m_responseData.requestStatus = Http::Status::NOT_IMPLEMENTED;
			m_fillFunction = &Response::mf_fillErrorResponse;
		}
	}

	BufferView
	Response::receiveRequestBody(const BufferView& view)
	{
		return ((this->*m_processFunction)(view));
	}


	Http::ResponseStatus::Type
	Response::fillWriteBuffer(BaseBuffer& writeBuffer)
	{


		// if cgi, call cgi to fill

		// call the current filling function
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
		if (m_cgiGateway)
		{
			Http::CgiInterface& cgiInterface =
			reinterpret_cast<Http::Module*>(m_context.getAppLayerModule(Ws::AppLayer::HTTP))->accessCgiInterface();
			cgiInterface.releaseGateway(*m_cgiGateway);
		}
		m_cgiGateway = NULL;
		m_connAddress = NULL;
	}

	void
	Response::setConnectionAddress(const Ws::Sock::addr& addr)
	{
		m_connAddress = &addr;
	}

}
