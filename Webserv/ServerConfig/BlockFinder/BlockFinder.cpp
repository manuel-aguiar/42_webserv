

#include "BlockFinder.hpp"

#include "../ServerBlock/ServerBlock.hpp"
#include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

BlockFinder::BlockFinder() {}

BlockFinder::~BlockFinder() {}

// Checks if a server block with the given server name exists
bool	BlockFinder::hasServerBlock(const std::string& serverName)
{
    return (findServerBlock(serverName) != NULL);
}

//	Adds a server block to the server_blocks map for each server name
void	BlockFinder::addServerBlock(const ServerBlock& block)
{
	const std::set<std::string>& serverNames = block.getServerNames();

	// For each server name
	for (std::set<std::string>::const_iterator it = serverNames.begin(); it != serverNames.end(); ++it)
	{
		// Add to map if not already present
		if (m_serverBlocks.find(*it) == m_serverBlocks.end())
			m_serverBlocks[*it] = &block;
	}
}

void    BlockFinder::loadServerBlocks(const std::vector<ServerBlock>& blocks)
{
    for (std::vector<ServerBlock>::const_iterator it = blocks.begin(); it != blocks.end(); ++it)
        addServerBlock(*it);
}

// findServerBlock Finds the server block that matches the given server name.
const ServerBlock*	BlockFinder::findServerBlock(const std::string& serverName)
{
	if (m_serverBlocks.empty())
		return (NULL);

	std::map<std::string, const ServerBlock*>::iterator it = m_serverBlocks.find(serverName);
	if (it != m_serverBlocks.end())
		return (it->second);

	return (NULL);
}

// Removes the server block that holds the given server name (and all other server names on the server block)
void	BlockFinder::removeServerBlock(const std::string& serverName)
{
    std::map<std::string , const ServerBlock*>::iterator sbIt = m_serverBlocks.find(serverName);
	
	if (sbIt == m_serverBlocks.end())
		return ;

	const std::set<std::string>& serverNames = sbIt->second->getServerNames();

	if (sbIt != m_serverBlocks.end())
		for (std::set<std::string>::const_iterator snIt = serverNames.begin(); snIt != serverNames.end(); ++snIt)
			m_serverBlocks.erase(*snIt);
}

// Removes a server name from the server_blocks map
void	BlockFinder::removeServerName(const std::string& serverName)
{
    std::map<std::string , const ServerBlock*>::iterator it = m_serverBlocks.find(serverName);

    if (it != m_serverBlocks.end())
        m_serverBlocks.erase(it);
}
