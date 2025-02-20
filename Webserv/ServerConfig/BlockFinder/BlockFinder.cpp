

#include "BlockFinder.hpp"

#include "../ServerBlock/ServerBlock.hpp"
#include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

BlockFinder::BlockFinder():
    m_wildcardKey() {
        m_wildcardKey.ip = 0;  // 0.0.0.0 in network byte order
        m_wildcardKey.port = 0; // * represented as 0
        m_wildcardKey.serverName = "*";
}

BlockFinder::~BlockFinder() {}

std::pair<uint32_t, uint16_t> BlockFinder::mf_extractIpPort(const Ws::Sock::addr* addr) const
{
    ASSERT_EQUAL(addr != NULL, true, "BlockFinder::mf_extractIpPort(): Address is NULL");
    ASSERT_EQUAL(addr->sa_family, AF_INET, "BlockFinder::mf_extractIpPort(): Currently supports only ipv4");

    Ws::Sock::addr_in* ipv4 = (Ws::Sock::addr_in*)addr;
    return std::make_pair(ipv4->sin_addr.s_addr, ipv4->sin_port);
}

/*
**	mf_createIdentifier
**	creates a BlockIdentifier struct for a given address and server name
**	uses normalized directives
*/
BlockFinder::BlockIdentifier BlockFinder::mf_createIdentifier(const Ws::Sock::addr* addr, const std::string& serverName) const {
    ASSERT_EQUAL(addr != NULL, true, "BlockFinder::mf_createIdentifier(): Address is NULL");

    BlockIdentifier key;
    std::pair<uint32_t, uint16_t> ipPort = mf_extractIpPort(addr);

    key.ip = ipPort.first;
    key.port = ipPort.second;
    key.serverName = serverName.empty() ? m_wildcardKey.serverName : serverName;

    return key;
}

/*
**	hasServerBlock
**	checks if a server block with the given ip, port and server name exists
**	uses normalized directives
*/
bool	BlockFinder::hasServerBlock(Ws::Sock::addr* address, const std::string& serverName)
{
    return (findServerBlock(address, serverName) != NULL);
}

/*
**	addServerBlock
**	adds a server block to the server_blocks map for each unique combination of listeners and server names
**	blocks are added with normalized directives
**  if a directive is not provided, falls back to the wildcard value
**	if the block is already in the map, it is not added again
*/
void	BlockFinder::addServerBlock(const ServerBlock& block)
{
    const std::vector<const Ws::Sock::addr*>& listeners = block.getListenAddresses();
    const std::set<std::string>& serverNames = block.getServerNames();

    // For each listener (IP:port pair)
    for (std::vector<const Ws::Sock::addr*>::const_iterator lit = listeners.begin(); lit != listeners.end(); ++lit)
    {
        // Port directive is mandatory and cannot be wildcard
        ASSERT_EQUAL(mf_extractIpPort(*lit).second != 0, true, "BlockFinder::addServerBlock(): Port directive is mandatory");

        // For each server name
        for (std::set<std::string>::const_iterator sit = serverNames.begin(); sit != serverNames.end(); ++sit)
        {
            BlockIdentifier key = mf_createIdentifier(*lit, *sit);

            // Add to map if not already present
            if (m_serverBlocks.find(key) == m_serverBlocks.end())
                m_serverBlocks[key] = &block;
        }
    }
}

void    BlockFinder::loadServerBlocks(const std::vector<ServerBlock>& blocks)
{
    for (std::vector<ServerBlock>::const_iterator it = blocks.begin(); it != blocks.end(); ++it)
        addServerBlock(*it);
}

/*
**	findServerBlock
**	Finds the server block that matches the given IP, port, and server name.
**	Returns a pointer to the server block if found, otherwise returns NULL.
**
**	The order of precedence for finding a matching block is as follows:
**	- Exact match for the combination of IP, port, and server name.
**	- IP:port match, but different server name.
**	- IP wildcard (0.0.0.0), but exact port and server name.
**	- IP:port match, but server name is a wildcard (*).
*/
const ServerBlock*	BlockFinder::findServerBlock(Ws::Sock::addr* address, const std::string& serverName)
{
    BlockIdentifier key = mf_createIdentifier(address, serverName);

    if (m_serverBlocks.empty())
        return (NULL);

    // Try exact match
    std::map<BlockIdentifier, const ServerBlock*>::iterator it = m_serverBlocks.find(key);
    if (it != m_serverBlocks.end())
        return (it->second);

    // Try wildcard server name
    key.serverName = m_wildcardKey.serverName;
    it = m_serverBlocks.find(key);
    if (it != m_serverBlocks.end())
        return (it->second);

    // Try wildcard IP with exact server name
    key = mf_createIdentifier(address, serverName);
    key.ip = m_wildcardKey.ip;
    it = m_serverBlocks.find(key);
    if (it != m_serverBlocks.end())
        return (it->second);

    return (NULL);
}

/*
**	removeServerBlock
**	removes a server block from the server_blocks map
*/
void	BlockFinder::removeServerBlock(Ws::Sock::addr* address, const std::string& serverName)
{
    BlockIdentifier key = mf_createIdentifier(address, serverName);
    std::map<BlockIdentifier, const ServerBlock*>::iterator it = m_serverBlocks.find(key);

    if (it != m_serverBlocks.end())
        m_serverBlocks.erase(it);
}

bool BlockFinder::BlockIdentifier::operator<(const BlockIdentifier& other) const {
    if (ip != other.ip) return ip < other.ip;
    if (port != other.port) return port < other.port;
    return serverName < other.serverName;
}
