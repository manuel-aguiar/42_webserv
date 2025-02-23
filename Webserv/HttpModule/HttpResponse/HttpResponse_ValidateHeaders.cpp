

# include "HttpResponse.hpp"

# include "../../ServerConfig/BlockFinder/BlockFinder.hpp"


namespace Http
{
    bool
    Response::mf_validateHeaders()
    {
        ASSERT_EQUAL(m_requestData != NULL, true, "Response: Request data not set");
        ASSERT_EQUAL(m_serverBlock, NULL, "Response: Server block alreadyset");
        ASSERT_EQUAL(m_connAddress != NULL, true, "Response: Connection address not set");

        std::map<RequestData::HeaderKey, RequestData::HeaderValue>::const_iterator host 
        = m_requestData->headers.find("Host");

        if (host == m_requestData->headers.end())
        {
            m_requestStatus = Http::Status::BAD_REQUEST;  //change to missing header
            return (false);
        }

        m_serverBlock = m_context.getBlockFinder()->findServerBlock(m_connAddress, host->second);

        // not implemented
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