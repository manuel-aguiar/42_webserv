#include "BlockFinder.hpp"

BlockFinder::BlockFinder():
    m_wildcardIp("0.0.0.0"),
    m_wildcardPort("*"), // defining this even though listen directive needs to be provided
    m_wildcardServerName("*") {}

BlockFinder::~BlockFinder() {}

bool    BlockFinder::mf_nonEmptyDirective(const std::string& str, const std::string& wildcard) const
{
    return (str.empty() || str == wildcard);
}

void    BlockFinder::mf_normalizeDirectives(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName)
{
    // assume values are correct by default
    m_normalizedIp = ip;
    m_normalizedPort = port;
    m_normalizedServerName = serverName;

    // if the directive is empty or is a wildcard, replace it with the wildcard value
    if (mf_nonEmptyDirective(ip, m_wildcardIp))
        m_normalizedIp = m_wildcardIp;
    if (mf_nonEmptyDirective(port, m_wildcardPort))
        m_normalizedPort = m_wildcardPort;
    if (mf_nonEmptyDirective(serverName, m_wildcardServerName))
        m_normalizedServerName = m_wildcardServerName;
}

std::string BlockFinder::mf_hashedKey(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName) const
{
    return (ip + ":" + port + ":" + serverName);
}

/*
**	hasServerBlock
**	checks if a server block with the given ip, port and server name exists
**	uses normalized directives
*/
bool	BlockFinder::hasServerBlock(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName)
{
    mf_normalizeDirectives(ip, port, serverName);
    if (this->findServerBlock(this->m_normalizedIp, this->m_normalizedPort, this->m_normalizedServerName))
        return (true);
    return (false);
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
    const std::set<t_listeners>& listeners = block.getListeners();
    const std::set<std::string>& serverNames = block.getServerNames();

    // For each listener (IP:port pair)
    for (std::set<t_listeners>::const_iterator lit = listeners.begin(); lit != listeners.end(); ++lit)
    {
        const t_ip_str& ip = lit->first;
        const t_port_str& port = lit->second;

        // Port directive is mandatory and cannot be wildcard
        assert(!port.empty() && "Port directive is mandatory");
        assert(port != m_wildcardPort && "Port directive cannot be a wildcard");

        // For each server name
        for (std::set<std::string>::const_iterator sit = serverNames.begin(); sit != serverNames.end(); ++sit)
        {
            const t_server_name& serverName = *sit;

            // Normalize and create key
            mf_normalizeDirectives(ip, port, serverName);
            std::string key = mf_hashedKey(m_normalizedIp, m_normalizedPort, m_normalizedServerName);

            // Add to map if not already present
            if (m_serverBlocks.find(key) == m_serverBlocks.end())
                m_serverBlocks[key] = &block;
        }
    }
}

/*
**	findServerBlock
**	Finds the server block that matches the given IP, port, and server name.
**	Returns a pointer to the server block if found, otherwise returns NULL.
**
**	The order of precedence for finding a matching block is as follows:
**	- Exact match for the combination of IP, port, and server name.
**	- IP:port match, but different server name.
**	- IP wildcard (`0.0.0.0`), but exact port and server name.
**	- IP:port match, but server name is a wildcard (`*`).
**	- IP wildcard (`0.0.0.0`), port wildcard (`*`), and exact server name.
**	- IP wildcard (`0.0.0.0`), port wildcard (`*`), and server name wildcard (`*`).
*/

const ServerBlock*	BlockFinder::findServerBlock(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName)
{
    std::string	key;

    if (m_serverBlocks.empty())
        return (NULL);

    mf_normalizeDirectives(ip, port, serverName);
    key = mf_hashedKey(this->m_normalizedIp, this->m_normalizedPort, this->m_normalizedServerName);

    if (m_serverBlocks.find(key) != m_serverBlocks.end())
        return (m_serverBlocks.find(key)->second);

    if (ip != m_normalizedIp)
        return (NULL);

    // if the specific ip didn't match, check if wildcard matches
    key = mf_hashedKey(m_wildcardIp, this->m_normalizedPort, this->m_normalizedServerName);
    std::map<std::string, const ServerBlock*>::iterator it = m_serverBlocks.find(key);

    if (it != m_serverBlocks.end())
        return (it->second);

    return (NULL);
}

/*
**	removeServerBlock
**	removes a server block from the server_blocks map
*/
void	BlockFinder::removeServerBlock(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName)
{
    std::string	key;

    mf_normalizeDirectives(ip, port, serverName);
    key = mf_hashedKey(this->m_normalizedIp, this->m_normalizedPort, this->m_normalizedServerName);

    if (m_serverBlocks.find(key) != m_serverBlocks.end())
        m_serverBlocks.erase(key);

    return;
}
