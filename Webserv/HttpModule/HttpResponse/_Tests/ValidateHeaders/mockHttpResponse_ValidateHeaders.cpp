# include "../../HttpResponse.hpp"

# include "../../../../ServerContext/ServerContext.hpp"
# include "../../../../ServerConfig/ServerBlock/ServerBlock.hpp"
# include "../../../../ServerConfig/ServerLocation/ServerLocation.hpp"
# include "../../../../ServerConfig/BlockFinder/BlockFinder.hpp"


bool returnValue;


namespace Http
{
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
		{
			std::cout << "Response: host header: " << host->second << std::endl;
			hostHeaderValue = host->second;
		}
		// Find ServerBlock
		m_responseData.serverBlock = m_context.getBlockFinder()->findServerBlock(*m_connAddress, hostHeaderValue);
		
		if (m_responseData.serverBlock == NULL)
		{
			m_responseData.requestStatus = Http::Status::NOT_FOUND;
			returnValue = false;
			std::cout << "Response: Server block not found" << std::endl;
			return (false);
		}
		
		// Find Location
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
			m_responseData.requestStatus = Http::Status::NOT_FOUND;
			returnValue = false;
			std::cout << "Response: Server location not found" << std::endl;
			return (false);
		}

		// Check file (extension, exists)
		m_responseData.targetPath = 
			m_responseData.serverBlock->getRoot() + 
			m_responseData.serverLocation->getRoot() + 
			m_responseData.requestData->path.substr(pathView.size());

		m_responseData.targetType = FilesUtils::getFileType(m_responseData.targetPath.c_str());

		switch (m_responseData.targetType)
		{
			case FilesUtils::DIRECTORY:
			{
				if (m_responseData.serverLocation->getAutoIndex())
				{
					m_responseData.requestStatus = Http::Status::OK; 
					break ;
				}
				m_responseData.requestStatus = Http::Status::FORBIDDEN;
				returnValue = false;
				return (false);
			}
			case FilesUtils::REGULAR_FILE:
			{
				// Check Accept header
				const std::map<RequestData::HeaderKey, RequestData::HeaderValue>::const_iterator accept
				= m_responseData.requestData->headers.find("Accept");

				if (accept != m_responseData.requestData->headers.end()
					&& !mf_validateAcceptType(m_responseData.requestData->headers.find("Accept")->second, m_responseData.targetPath))
				{
					m_responseData.requestStatus = Http::Status::NOT_ACCEPTABLE;
					returnValue = false;
					return (false);
				}

				m_responseData.requestStatus = Http::Status::OK;
				break ;
			}
			default:
				m_responseData.requestStatus = Http::Status::NOT_FOUND;  // change to the right error code
				returnValue = false;
				return (false);
		}

		std::map<RequestData::HeaderKey, RequestData::HeaderValue>::const_iterator connection 
		= m_responseData.requestData->headers.find("Connection");

		if (connection != m_responseData.requestData->headers.end() && connection->second == "close")
			m_responseData.closeAfterSending = true;
		returnValue = true;
		return (true);
	}
}
