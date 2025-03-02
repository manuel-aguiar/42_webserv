# include "HttpResponse.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../ServerConfig/ServerBlock/ServerBlock.hpp"
# include "../../ServerConfig/ServerLocation/ServerLocation.hpp"
# include "../../ServerConfig/BlockFinder/BlockFinder.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"

# include <arpa/inet.h>

namespace Http
{
	// Check Server/Location existence
	// Check file availability
	// Check Method allowed
	// Check Host, Accept, Connection header
	bool
	Response::mf_validateHeaders()
	{

		ASSERT_EQUAL(m_responseData.requestData != NULL, true, "Response: Request data not set");
		ASSERT_EQUAL(m_responseData.serverBlock, (const ServerBlock*)NULL, "Response: Server block alreadyset");
		ASSERT_EQUAL(m_connAddress != NULL, true, "Response: Connection address not set");

		// Host header
		std::string hostHeaderValue;
		std::map<RequestData::HeaderKey, RequestData::HeaderValue>::const_iterator host
		= m_responseData.requestData->headers.find("Host");

		if (host != m_responseData.requestData->headers.end())
			hostHeaderValue = host->second;

		// Find ServerBlock
		m_responseData.serverBlock = m_context.getBlockFinder()->findServerBlock(*m_connAddress, hostHeaderValue);

		if (m_responseData.serverBlock == NULL)
		{
			m_responseData.requestStatus = Http::Status::NOT_FOUND; // error
			m_responseData.responseType = ResponseData::ERROR;
			return (false);
		}

		// Find Location
		mf_findLocation(m_responseData);

		// Check Request Method Permission (No location, no method check. Everything goes!)
		if (m_responseData.serverLocation != NULL
			&& m_responseData.serverLocation->getMethods().find(m_responseData.requestData->method) == m_responseData.serverLocation->getMethods().end())
		{
			// We could as per spec send Allow header with available options
			m_responseData.requestStatus = Http::Status::METHOD_NOT_ALLOWED; // error
			m_responseData.responseType = ResponseData::ERROR;
			return (false);
		}

		// Check redirection
		if (m_responseData.serverLocation != NULL)
		{
			const bool isRedirect = mf_checkRedirect();
			if (isRedirect)
				return (false);
		}

		// Assemble target path
        // ROOT MUST BE DIRECTORY & not end with '/'
		// Assemble target path using alias behavior
		mf_assembleTargetPath();

		// Check resource (exists, extension)
		m_responseData.targetType = FilesUtils::getFileType(m_responseData.targetPath.c_str());
		std::map<RequestData::HeaderKey, RequestData::HeaderValue>::const_iterator acceptHeader;

		switch (m_responseData.targetType)
		{
			case FilesUtils::DIRECTORY:
				if (*m_responseData.targetPath.rbegin() != '/')
				{
					// redirect to same path with '/' in the end ??
					// m_responseData.requestStatus = Http::Status::MOVED_PERMANENTLY;
					// return (false);
					// TODO: check if this is how we should handle this
					m_responseData.targetPath += "/";
				}

				// Upload
				if (m_responseData.requestData->method == "POST"
					&& m_responseData.requestData->headers.find("Content-Type")->second == "multipart/form-data")
				{
					m_responseData.responseType = ResponseData::FILE_UPLOAD;
					break ;
				}

				// Autoindex default is 0, so if we dont have a location, 403.
				// Can we access config defaults from here?
				if (m_responseData.serverLocation != NULL
					&& m_responseData.requestData->method == "GET"
					&& m_responseData.serverLocation->getAutoIndex() == 1)
				{
					m_responseData.responseType = ResponseData::DIRECTORY_LISTING;
					break ;
				}
				m_responseData.requestStatus = Http::Status::FORBIDDEN;
				m_responseData.responseType = ResponseData::ERROR;
				return (false); // error
			case FilesUtils::REGULAR_FILE:
				m_responseData.targetExtension = StringUtils::extractFileExtension(m_responseData.targetPath);

				if (m_responseData.serverLocation != NULL
					&& m_responseData.serverLocation->getCgiInterpreters().find(m_responseData.targetExtension) != m_responseData.serverLocation->getCgiInterpreters().end())
				{
					m_responseData.responseType = ResponseData::CGI;
					return (true);
				}

				// TODO: GET -> open the file
				// TODO: DELETE -> delete the file

				// Check Accept header
				acceptHeader = m_responseData.requestData->headers.find("accept");

				if (acceptHeader != m_responseData.requestData->headers.end()
					&& !mf_validateAcceptType(m_responseData.requestData->headers.find("accept")->second, m_responseData.targetPath))
				{
					m_responseData.requestStatus = Http::Status::NOT_ACCEPTABLE;
					m_responseData.responseType = ResponseData::ERROR;
					return (false);
				}

				m_responseData.responseType = ResponseData::STATIC;
				break ;
			case FilesUtils::UNDEFINED:
				m_responseData.requestStatus = Http::Status::NOT_FOUND; // ???
				return (false);
			case FilesUtils::NOT_EXIST:
				//same as above
				if (m_responseData.serverLocation != NULL
					&& m_responseData.requestData->method == "GET"
					&& m_responseData.serverLocation->getAutoIndex() == 1)
				{
					m_responseData.targetPath = m_responseData.targetPath.substr(0, m_responseData.targetPath.length() - m_responseData.serverLocation->getIndex().length());
					break ;
				}
				/* fall through */
			default:
				m_responseData.requestStatus = Http::Status::NOT_FOUND;
				m_responseData.responseType = ResponseData::ERROR;
				return (false);
		}

		std::map<RequestData::HeaderKey, RequestData::HeaderValue>::const_iterator connection
		= m_responseData.requestData->headers.find("connection");

		if (connection != m_responseData.requestData->headers.end() && connection->second == "close")
			m_responseData.closeAfterSending = true;

		return (true);
	}
}
