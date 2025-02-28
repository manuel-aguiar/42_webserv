#include "HttpResponse.hpp"
#include "../../ServerContext/ServerContext.hpp"
#include "../../ServerConfig/ServerBlock/ServerBlock.hpp"
#include "../../ServerConfig/ServerLocation/ServerLocation.hpp"
#include "../../ServerConfig/BlockFinder/BlockFinder.hpp"

#include <vector>

namespace Http
{
	// Find the most specific matching location
	ServerLocation* Response::mf_findLocation(ResponseData& responseData)
	{
		const std::vector<ServerLocation>& locations = responseData.serverBlock->getLocations();
		BufferView pathView(responseData.requestData->path);

		// Try exact match first
		for (size_t i = 0; i < locations.size(); ++i) {
			const std::string& locationPath = locations[i].getPath();
			if (BufferView(locationPath.c_str(), locationPath.size()) == pathView) {
				m_responseData.serverLocation = &locations[i];
				break;
			}
		}

		// If no exact match, try parent paths
		if (!m_responseData.serverLocation) {
			size_t longestMatch = 0;
			std::string path(pathView.data(), pathView.size());
			
			while (!path.empty()) {
				size_t lastSlash = path.find_last_of('/');
				if (lastSlash == std::string::npos) break;
				
				path = path.substr(0, lastSlash);
				if (path.empty()) path = "/";

				for (size_t i = 0; i < locations.size(); ++i) {
					const std::string& locationPath = locations[i].getPath();
					if (locationPath == path && locationPath.size() > longestMatch) {
						longestMatch = locationPath.size();
						m_responseData.serverLocation = &locations[i];
					}
				}
				
				if (path == "/") break;
			}
		}
	}
}
