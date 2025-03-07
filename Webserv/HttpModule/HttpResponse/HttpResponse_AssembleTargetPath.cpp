
#include "HttpResponse.hpp"
#include "../../ServerContext/ServerContext.hpp"
#include "../../ServerConfig/ServerBlock/ServerBlock.hpp"
#include "../../ServerConfig/ServerLocation/ServerLocation.hpp"

namespace Http
{
	void Response::mf_assembleTargetPath()
	{
		if (m_responseData.serverLocation == NULL || m_responseData.serverLocation->getRoot().empty()) 
		{
			// No location or no root/alias - use server root
			m_responseData.targetPath = m_responseData.serverBlock->getRoot();
			if (*m_responseData.targetPath.rbegin() == '/')
				m_responseData.targetPath.erase(m_responseData.targetPath.size() - 1);
			m_responseData.targetPath += m_responseData.requestData->path;
		}
		else
		{
			// Remove the matched location prefix from request path
			std::string remainingPath = m_responseData.requestData->path.substr(m_responseData.serverLocation->getPath().length());
			
			// Construct final path using alias
			m_responseData.targetPath = m_responseData.serverLocation->getRoot();
			if (*m_responseData.targetPath.rbegin() == '/')
				m_responseData.targetPath.erase(m_responseData.targetPath.size() - 1);
			if (!remainingPath.empty())
			{
				if (*remainingPath.begin() != '/')
					m_responseData.targetPath += "/";
				m_responseData.targetPath += remainingPath;
			}
			else if (m_responseData.requestData->path == "/")
				m_responseData.targetPath += "/";
		}

		if (FilesUtils::isDirectory(m_responseData.targetPath.c_str())
			&& m_responseData.serverLocation != NULL
			&& *m_responseData.requestData->path.rbegin() == '/'
			&& !m_responseData.serverLocation->getIndex().empty()) 
			{
			m_responseData.targetPath += m_responseData.serverLocation->getIndex();
			m_responseData.indexAppended = true;
		}
	}
}

// Root directive logic
// if (m_responseData.serverLocation == NULL || m_responseData.serverLocation->getRoot().empty()) 
// 	m_responseData.targetPath = m_responseData.serverBlock->getRoot();

// if (*m_responseData.targetPath.rbegin() == '/')
// 	m_responseData.targetPath.erase(m_responseData.targetPath.size() - 1);

// m_responseData.targetPath += m_responseData.requestData->path;

// if (m_responseData.serverLocation != NULL
// 	&& *m_responseData.requestData->path.rbegin() == '/'
// 	&& !m_responseData.serverLocation->getIndex().empty()) 
// {
// 	targetPathNoIndex = m_responseData.targetPath;
// 	m_responseData.targetPath += m_responseData.serverLocation->getIndex();
// }
