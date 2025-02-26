

# include "../../HttpResponse.hpp"

# include "../../../../ServerContext/ServerContext.hpp"
# include "../../../../ServerConfig/ServerBlock/ServerBlock.hpp"
# include "../../../../ServerConfig/ServerLocation/ServerLocation.hpp"
# include "../../../../ServerConfig/BlockFinder/BlockFinder.hpp"


namespace Http
{
	bool
	Response::mf_validateHeaders()
	{
		m_processFunction = &Response::mf_processBodyUpload;
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
