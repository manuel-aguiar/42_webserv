

#include "HttpResponse.hpp"
#include "../HttpModule/HttpModule.hpp"
#include "../HttpCgiInterface/HttpCgiInterface.hpp"
#include "../../ServerConfig/BlockFinder/BlockFinder.hpp"
#include "../../ServerContext/ServerContext.hpp"
#include "../../GenericUtils/Files/FilesUtils.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../GenericUtils/Buffer/Buffer.hpp"
#include "../../ServerConfig/ServerBlock/ServerBlock.hpp"

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

// Move to adequate scope
#define SERVER_NAME_VERSION "42_webserv/1.0"

// TODO: move to response scope
extern std::string DirectoryListing(const std::string& path);
extern std::string getMimeType(const std::string &path);

namespace Http
{

	void	Response::mf_resolveRequestData()
	{	
		if (m_responseData.requestStatus != Http::Status::OK 
			|| !mf_resolveServerAndLocation()
			|| !mf_checkPermissions()
			|| mf_checkRedirect())
			return ;

		mf_validateTargetPath();
	}

	void	Response::receiveRequestData(const Http::RequestData& data)
	{
		m_responseData.requestData = &data;
		m_responseData.requestStatus = data.status;
		m_fillFunction = &Response::mf_fillResponseLine;

		mf_resolveRequestData();

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

		std::cout << "responseType: " << m_responseData.responseType << std::endl;
		
		switch (m_responseData.responseType)
		{
			case ResponseData::STATIC:
				mf_addContentHeaders(m_file.size(), getMimeType(m_responseData.targetPath));

				if (mf_addCacheControlHeaders())
					m_fillFunctionBody = &Response::mf_fillFinish;
				else
					m_fillFunctionBody = &Response::mf_sendStaticFile;
				break ;
			case ResponseData::CGI:
				return (mf_prepareCgiExecution());
			case ResponseData::REDIRECT:
				m_defaultPageContent = mf_generateRedirectPage(m_responseData.requestStatus, m_responseData.headers["location"]);
				mf_addContentHeaders(m_defaultPageContent.size(), "text/html");
				m_fillFunctionBody = &Response::mf_fillRedirect;
				break ;
			case ResponseData::DIRECTORY_LISTING: // Directory Listing and Error have similar behavior
				//m_defaultPageContent = DirectoryListing(m_responseData.targetPath);

				m_responseData.headers.insert(std::make_pair("content-type", "text/html"));
				m_responseData.headers.insert(std::make_pair("transfer-encoding", "chunked"));
				m_fillFunctionBody = &Response::mf_fillDirectoryListing_Head;
				break ;
			case ResponseData::FILE_UPLOAD:
				m_fillFunction = &Response::mf_fillNothingToSend;
				m_processFunction = &Response::mf_processBodyUpload;
				break ;
			case ResponseData::ERROR:
				m_processFunction = &Response::mf_processBodyIgnore;
				mf_prepareErrorMessage();
				break ;
			case ResponseData::NO_CONTENT:
				mf_addHeader("content-length", "0");
				m_fillFunctionBody = &Response::mf_fillFinish;
				break ;
			default:
				break ;
		}

		m_currentHeader = m_responseData.headers.begin();

		return ;
	}

	BufferView
	Response::receiveRequestBody(const BufferView& view)
	{
		//std::cout << "Response received body, size: " << view.size() << std::endl;
		return ((this->*m_processFunction)(view));
	}


	Http::IOStatus::Type
	Response::fillWriteBuffer(BaseBuffer& writeBuffer)
	{
		if (m_ioStatus == Http::IOStatus::FINISHED || 
			m_ioStatus == Http::IOStatus::MARK_TO_CLOSE)
			return (m_ioStatus);
		m_ioStatus = (this->*m_fillFunction)(writeBuffer);
		return (m_ioStatus);
	}

	Http::IOStatus::Type
	Response::getStatus() const
	{
		return (m_ioStatus);
	}

	Http::IOStatus::Type
	Response::write()
	{
		Http::IOStatus::Type responseStatus = fillWriteBuffer(*m_writeBuffer);

		int bytesWritten = m_writeBuffer->write(m_writeFd); // write all you can, from the beginning
		
		if (bytesWritten == -1)
			return (Http::IOStatus::MARK_TO_CLOSE);
		
		m_writeBuffer->truncatePush(m_writeBuffer->view().substr(bytesWritten, m_writeBuffer->size() - bytesWritten));

		if (m_writeBuffer->size() == 0)
			return (responseStatus);

		return (Http::IOStatus::WAITING);
	}

	void
	Response::reset()
	{
		m_responseData.reset();
		m_fillFunction = &Response::mf_fillNothingToSend;
		m_fillFunctionBody = NULL;
		m_processFunction = &Response::mf_processBodyNone;
		m_pendingWrite.clear();
		m_ioStatus = IOStatus::WAITING;
		m_staticReadCounter = 0;
		m_file.reset();
		if (m_cgiResponse)
		{
			Http::CgiInterface& cgiInterface =
			reinterpret_cast<Http::Module*>(m_context.getAppLayerModule(Ws::AppLayer::HTTP))->accessCgiInterface();
			cgiInterface.releaseGateway(*m_cgiResponse);
			m_cgiResponse = NULL;
			m_cgiResponse = NULL;
		}
		if (m_dirListing_target)
		{
			::closedir(m_dirListing_target);
			m_dirListing_target = NULL;
		}
		m_dirListing_curEntry = NULL;
	}

	void
	Response::close()
	{
		reset();
		m_writeBuffer = NULL;
		m_writeFd = Ws::FD_NONE;
		m_listenAddress = NULL;
		m_tcpConn = NULL;
	}	

	void
	Response::setListenAddress(const Ws::Sock::addr& addr)
	{
		m_listenAddress = &addr;
	}

	void
	Response::setTcpConnection(const Conn::Connection& tcpConn)
	{
		m_tcpConn = &tcpConn;
	}

}
