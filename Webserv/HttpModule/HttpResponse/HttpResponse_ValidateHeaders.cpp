

# include "HttpResponse.hpp"

# include "../../ServerContext/ServerContext.hpp"
# include "../../ServerConfig/ServerBlock/ServerBlock.hpp"
# include "../../ServerConfig/ServerLocation/ServerLocation.hpp"
# include "../../ServerConfig/BlockFinder/BlockFinder.hpp"


namespace Http
{
	bool
	Response::mf_validateHeaders()
	{
		ASSERT_EQUAL(m_responseData.requestData != NULL, true, "Response: Request data not set");
		ASSERT_EQUAL(m_responseData.serverBlock, (const ServerBlock*)NULL, "Response: Server block alreadyset");
		ASSERT_EQUAL(m_connAddress != NULL, true, "Response: Connection address not set");

		std::map<RequestData::HeaderKey, RequestData::HeaderValue>::const_iterator host 
		= m_responseData.requestData->headers.find("Host");

		if (host == m_responseData.requestData->headers.end())
		{
			m_responseData.requestStatus = Http::Status::BAD_REQUEST;  //change to missing header
			return (false);
		}

		m_responseData.serverBlock = m_context.getBlockFinder()->findServerBlock(*m_connAddress, host->second);

		if (m_responseData.serverBlock == NULL)
		{
			m_responseData.requestStatus = Http::Status::NOT_FOUND;  // change to the right error code
			return (false);
		}

		const std::vector<ServerLocation>& locations = m_responseData.serverBlock->getLocations();
		BufferView pathView(m_responseData.requestData->path);

		for (size_t i = 0; i < locations.size(); ++i)
		{
			const std::string& locationPath = locations[i].getPath();
			if (BufferView(locationPath.c_str(), pathView.size()) == pathView &&
			(
				locationPath.size() == pathView.size() ||
				locationPath[pathView.size()] == '/'
			))
			{
				m_responseData.serverLocation = &locations[i];
				break ;
			}
			
		}

		if (m_responseData.serverLocation == NULL)
		{
			m_responseData.requestStatus = Http::Status::NOT_FOUND;  // change to the right error code
			return (false);
		}

		m_responseData.targetPath = 
			m_responseData.serverBlock->getRoot() + 
			m_responseData.serverLocation->getRoot() + 
			m_responseData.requestData->path.substr(pathView.size());

		m_responseData.targetType = FilesUtils::getFileType(m_responseData.targetPath.c_str());

		// ACCEPT
		
		switch (m_responseData.targetType)
		{
			case FilesUtils::DIRECTORY:
			{
				if (m_responseData.serverLocation->getAutoIndex())
				{
					m_responseData.requestStatus = Http::Status::OK; 
					break ;
				}
				m_responseData.requestStatus = Http::Status::FORBIDDEN;  // change to the right error code
				return (false);
			}
			case FilesUtils::REGULAR_FILE:
			{
				m_responseData.requestStatus = Http::Status::OK;
				break ;
			}
			default:
				m_responseData.requestStatus = Http::Status::NOT_FOUND;  // change to the right error code
				return (false);
		}

		std::map<RequestData::HeaderKey, RequestData::HeaderValue>::const_iterator keepAlive 
		= m_responseData.requestData->headers.find("Connection");

		if (keepAlive != m_responseData.requestData->headers.end() && keepAlive->second == "close")
			m_responseData.closeAfterSending = true;

		// not implemented
		// CHECK FOR ACCEPT, FILE TYPES


		return (true);
	}

}











	/****** These checks go to request ********************************** */
	// Get server block
	//m_serverBlock = context.getBlockFinder()->findServerBlock(myConnection.getAddr(), m_myRequest.getHost());
	// if (m_serverBlock == NULL)
	// {
	// 	std::cerr << "DEBUG: " << "ServerBlock Not Found!\n";
	// 	this->mf_generateResponse(Http::Status::NOT_FOUND);
	// // get location
	// if (m_serverBlock->getLocations().find(m_myRequest.getUri()) == m_serverBlock->getLocations().end())
	
	// check for file (extension, exists)
	// if (FilesUtils::isFile(m_myRequest.getUri().c_str()))
	// {
	// 	std::cerr << "DEBUG: " << "Requested URI is File\n";
	// 	this->mf_generateResponse(Http::Status::OK);
	// }
	// else if (FilesUtils::isDirectory(m_myRequest.getUri().c_str()))
	// {
	// 	std::cerr << "DEBUG: " << "Requested URI is Directory\n";
	// 	// check auto_index on serverConfig
	// 	this->mf_generateResponse(Http::Status::FORBIDDEN);
	// }
	// else
	// {
	// 	std::cerr << "DEBUG: " << "Requested URI Not Found\n";
	// 	this->mf_generateResponse(Http::Status::NOT_FOUND);
	// }

	/****************************************************************** */

	// NOTE: mf_generateResponse() will be called at the end of http::request parsing process and not here
	
	// ParsingIncomplete -> Wait
	//if (myRequest.getParsingState() != Http::Request::ERROR && myRequest.getParsingState() != Http::Request::COMPLETED)
	//	return ;
	//// Request parsing/execution finished -> Generate response.
	//mf_generateResponse(myRequest.getStatus());	// 	return ;
	// }// {
	// 	std::cerr << "DEBUG: " << "Location Not Found!\n";
	// 	this->mf_generateResponse(Http::Status::NOT_FOUND);
	// 	return ;
	// }
	// m_location = m_serverBlock->getLocations().find(m_myRequest.getUri())->second;

	// check for file (extension, exists)
	// if (FilesUtils::isFile(m_myRequest.getUri().c_str()))
	// {
	// 	std::cerr << "DEBUG: " << "Requested URI is File\n";
	// 	this->mf_generateResponse(Http::Status::OK);
	// }
	// else if (FilesUtils::isDirectory(m_myRequest.getUri().c_str()))
	// {
	// 	std::cerr << "DEBUG: " << "Requested URI is Directory\n";
	// 	// check auto_index on serverConfig
	// 	this->mf_generateResponse(Http::Status::FORBIDDEN);
	// }
	// else
	// {
	// 	std::cerr << "DEBUG: " << "Requested URI Not Found\n";
	// 	this->mf_generateResponse(Http::Status::NOT_FOUND);
	// }

	/****************************************************************** */

	// NOTE: mf_generateResponse() will be called at the end of http::request parsing process and not here
	
	// ParsingIncomplete -> Wait
	//if (myRequest.getParsingState() != Http::Request::ERROR && myRequest.getParsingState() != Http::Request::COMPLETED)
	//	return ;
	//// Request parsing/execution finished -> Generate response.
	//mf_generateResponse(myRequest.getStatus());	// 	return ;
	// }