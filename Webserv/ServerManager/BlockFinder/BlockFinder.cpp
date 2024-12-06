#include "BlockFinder.hpp"

BlockFinder::BlockFinder(const ServerConfig& config):
    m_config(config),
    m_wildcard_ip("0.0.0.0"),
    m_wildcard_port("*"), // defining this even though listen directive needs to be provided
    m_wildcard_server_name("*") {}

BlockFinder::~BlockFinder() {}

bool    BlockFinder::nonEmptyDirective(const std::string &str, const std::string &wildcard)
{
    return (str.empty() || str == wildcard);
}

void    BlockFinder::normalizeDirectives(t_ip_str &ip, t_port_str &port, t_server_name &server_name)
{
    // if the directive is empty or is a wildcard, replace it with the wildcard value
    if (nonEmptyDirective(ip, m_wildcard_ip))
        ip = m_wildcard_ip;
    if (nonEmptyDirective(port, m_wildcard_port))
        port = m_wildcard_port;
    if (nonEmptyDirective(server_name, m_wildcard_server_name))
        server_name = m_wildcard_server_name;
}

std::string	BlockFinder::hashedKey(t_ip_str ip, t_port_str port, t_server_name server_name)
{
    return (std::string(ip + ":" + port + ":" + server_name));
}

/*
**	hasServerBlock
**	checks if a server block with the given ip, port and server name exists
**	returns true if it exists, otherwise returns false
*/
bool	BlockFinder::hasServerBlock(t_ip_str ip, t_port_str port, t_server_name server_name)
{
    normalizeDirectives(ip, port, server_name);
    if (this->findServerBlock(ip, port, server_name))
        return (true);
    return (false);
}

/*
**	addServerBlock
**	adds a server block to the unordered_map
**	blocks are added with normalized directives
**	if the block is already in the map, it is not added again
*/
void	BlockFinder::addServerBlock(const ServerBlocks& block, t_ip_str ip, t_port_str port, t_server_name server_name)
{
    if (this->hasServerBlock(ip, port, server_name))
        return;

    std::string key = hashedKey(ip, port, server_name);
    m_server_blocks[key] = &block;
}

/*
**	findServerBlock
**	finds the server block that matches the given ip, port and server name
**	returns a pointer to the server block if found, otherwise returns NULL

**	The order of precedence is:
**	1. exact match of the combination of ip, port and server name
**	2. wildcard ip and port, but exact server name
**	3. wildcard ip, but exact port and server name
**	4. wildcard ip and server name, but exact port
*/
const ServerBlocks*	BlockFinder::findServerBlock(t_ip_str ip, t_port_str port, t_server_name server_name)
{
    if (m_server_blocks.empty())
        return (NULL);

    normalizeDirectives(ip, port, server_name);

    std::string key = hashedKey(ip, port, server_name);
    return (m_server_blocks[key]);
}
