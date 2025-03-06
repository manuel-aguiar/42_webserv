

# include "HttpResponse.hpp"
# include "../HttpModule/HttpModule.hpp"
# include "../HttpCgiInterface/HttpCgiInterface.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../ServerConfig/ServerBlock/ServerBlock.hpp"
# include "../../ServerConfig/ServerLocation/ServerLocation.hpp"
# include "../../ServerConfig/BlockFinder/BlockFinder.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"

# include <arpa/inet.h>

# include <cstdlib> // DELETE ME

extern std::string getMimeType(const std::string &path);

namespace Http
{

	bool	Response::mf_resolveServerAndLocation()
	{
		ASSERT_EQUAL(m_responseData.requestData != NULL, true, "Response: Request data not set");
		ASSERT_EQUAL(m_responseData.serverBlock, (const ServerBlock*)NULL, "Response: Server block alreadyset");
		ASSERT_EQUAL(m_listenAddress != NULL, true, "Response: Connection address not set");

		std::map<RequestData::HeaderKey, RequestData::HeaderValue>::const_iterator connection
		= m_responseData.requestData->headers.find("Connection");

		if (connection != m_responseData.requestData->headers.end() && connection->second == "close")
			m_responseData.closeAfterSending = true;

		// Host header
		std::string hostHeaderValue;
		std::map<RequestData::HeaderKey, RequestData::HeaderValue>::const_iterator host
		= m_responseData.requestData->headers.find("Host");

		if (host != m_responseData.requestData->headers.end())
			hostHeaderValue = host->second;

		// Find ServerBlock
		m_responseData.serverBlock = m_context.getBlockFinder()->findServerBlock(*m_listenAddress, hostHeaderValue);

		if (m_responseData.serverBlock == NULL)
		{
			m_responseData.requestStatus = Http::Status::NOT_FOUND;
			m_responseData.responseType = ResponseData::ERROR;
			return (false);
		}

		// Find Location
		mf_findLocation(m_responseData);

		return (true);
	}

	// Check Request Method Permission (No location, no method check. Everything goes!)
	bool Response::mf_checkPermissions()
	{
		if (m_responseData.serverLocation != NULL
			&& m_responseData.serverLocation->getMethods().find(m_responseData.requestData->method) == m_responseData.serverLocation->getMethods().end())
		{
			// We could as per spec send Allow header with available options
			m_responseData.requestStatus = Http::Status::METHOD_NOT_ALLOWED;
			m_responseData.responseType = ResponseData::ERROR;
			return (false);
		}

		return (true);
	}

	void Response::mf_validateTargetPath()
	{
		mf_assembleTargetPath();

		m_responseData.targetType = FilesUtils::getFileType(m_responseData.targetPath.c_str());
		std::map<RequestData::HeaderKey, RequestData::HeaderValue>::const_iterator acceptHeader;

		switch (m_responseData.targetType)
		{
			case FilesUtils::REGULAR_FILE:
				m_responseData.targetExtension = StringUtils::extractFileExtension(m_responseData.targetPath);
				if (m_responseData.serverLocation != NULL
					&& m_responseData.serverLocation->getCgiInterpreters().find(m_responseData.targetExtension) != m_responseData.serverLocation->getCgiInterpreters().end())
				{
					m_responseData.responseType = ResponseData::CGI;
					return ;
				}
				if (m_responseData.requestData->method == "GET")
				{
					// Check Accept header
					acceptHeader = m_responseData.requestData->headers.find("Accept");

					if (acceptHeader != m_responseData.requestData->headers.end()
						&& !mf_validateAcceptType(acceptHeader->second, m_responseData.targetPath))
					{
						m_responseData.requestStatus = Http::Status::NOT_ACCEPTABLE;
						m_responseData.responseType = ResponseData::ERROR;
						return ;
					}
					// try to open file
					if (mf_prepareStaticFile(m_responseData.targetPath.c_str()))
					{
						m_responseData.responseType = ResponseData::STATIC;
						return ;
					}
					else
					{
						m_responseData.requestStatus = Http::Status::INTERNAL_ERROR; // Check Error Code
						m_responseData.responseType = ResponseData::ERROR;
						return ;
					}
				}
				else if (m_responseData.requestData->method == "DELETE")
				{
					if (!unlink(m_responseData.targetPath.c_str()))
					{
						m_responseData.requestStatus = Http::Status::NO_CONTENT;
						m_responseData.responseType = ResponseData::STATIC;
					}
					else
					{
						m_responseData.requestStatus = Http::Status::INTERNAL_ERROR;
						m_responseData.responseType = ResponseData::ERROR;
						return ;
					}
				}
				m_responseData.responseType = ResponseData::STATIC;
				break ;
			case FilesUtils::DIRECTORY:
				if (*m_responseData.targetPath.rbegin() != '/')
				{
					// redirect to same path with '/' in the end
					m_responseData.responseType = ResponseData::REDIRECT;
					m_responseData.requestStatus = Http::Status::MOVED_PERMANENTLY;
					mf_addHeader("Location", m_responseData.requestData->path + "/");
					return ;
				}
				// Upload
				if (m_responseData.requestData->method == "POST"
					&& m_responseData.requestData->headers.find("Content-Type")->second.compare(0, 19, "multipart/form-data") == 0) // Please review this comparison
				{
					m_responseData.responseType = ResponseData::FILE_UPLOAD;
					return ;
				}
				// Autoindex default is 0, so if we dont have a location, 403.
				// Can we access config defaults from here?
				if (m_responseData.serverLocation != NULL
					&& m_responseData.requestData->method == "GET"
					&& m_responseData.serverLocation->getAutoIndex() == 1)
				{
					m_responseData.responseType = ResponseData::DIRECTORY_LISTING;
					return ;
				}
				m_responseData.requestStatus = Http::Status::FORBIDDEN;
				m_responseData.responseType = ResponseData::ERROR;
				return ;
			case FilesUtils::NOT_EXIST:
				if (m_responseData.indexAppended)
				{
					m_responseData.targetPath = m_responseData.targetPath.substr(0, m_responseData.targetPath.length() - m_responseData.serverLocation->getIndex().length());
					m_responseData.responseType = ResponseData::DIRECTORY_LISTING;
					return ;
				}
				m_responseData.requestStatus = Http::Status::NOT_FOUND;
				m_responseData.responseType = ResponseData::ERROR;
				return ;
			case FilesUtils::UNDEFINED:
				m_responseData.requestStatus = Http::Status::INTERNAL_ERROR;
				m_responseData.responseType = ResponseData::ERROR;
				return ;
		}
	}
}
