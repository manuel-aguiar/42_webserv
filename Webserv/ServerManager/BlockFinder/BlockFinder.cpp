#include "BlockFinder.hpp"

BlockFinder::BlockFinder(const ServerConfig& config):
    m_config(config),
    m_wildcard_ip("0.0.0.0"),
    m_wildcard_port("*"), // defining this even though listen directive needs to be provided
    m_wildcard_server_name("*") {}

BlockFinder::~BlockFinder() {}

bool    BlockFinder::mf_nonEmptyDirective(const std::string &str, const std::string &wildcard)
{
    return (str.empty() || str == wildcard);
}

void    BlockFinder::mf_normalizeDirectives(t_ip_str &ip, t_port_str &port, t_server_name &server_name)
{
    // if the directive is empty or is a wildcard, replace it with the wildcard value
    if (mf_nonEmptyDirective(ip, m_wildcard_ip))
        ip = m_wildcard_ip;
    if (mf_nonEmptyDirective(port, m_wildcard_port))
        port = m_wildcard_port;
    if (mf_nonEmptyDirective(server_name, m_wildcard_server_name))
        server_name = m_wildcard_server_name;
}

std::string	BlockFinder::mf_hashedKey(t_ip_str ip, t_port_str port, t_server_name server_name)
{
    return (std::string(ip + ":" + port + ":" + server_name));
}

/*
**	hasServerBlock
**	checks if a server block with the given ip, port and server name exists
**	uses normalized directives
*/
bool	BlockFinder::hasServerBlock(t_ip_str ip, t_port_str port, t_server_name server_name)
{
    mf_normalizeDirectives(ip, port, server_name);
    if (this->findServerBlock(ip, port, server_name))
        return (true);
    return (false);
}

/*
**	addServerBlock
**	adds a server block to the server_blocks map
**	blocks are added with normalized directives
**  if a directive is not provided, falls back to the wildcard value
**	if the block is already in the map, it is not added again
*/
void	BlockFinder::addServerBlock(const ServerBlocks& block, t_ip_str ip, t_port_str port, t_server_name server_name)
{
    // listen directive in the config is mandatory, so asserting the port here
    assert(!port.empty() && "Port directive is mandatory");
    assert(port != m_wildcard_port && "Port directive cannot be a wildcard");

    std::string	key;

    if (this->hasServerBlock(ip, port, server_name))
        return;

    mf_normalizeDirectives(ip, port, server_name);
    key = mf_hashedKey(ip, port, server_name);

    m_server_blocks[key] = &block;
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

const ServerBlocks*	BlockFinder::findServerBlock(t_ip_str ip, t_port_str port, t_server_name server_name)
{
    std::string	key;

    if (m_server_blocks.empty())
        return (NULL);

    mf_normalizeDirectives(ip, port, server_name);
    key = mf_hashedKey(ip, port, server_name);

    if (m_server_blocks.find(key) != m_server_blocks.end())
        return (m_server_blocks[key]);

    return (NULL);
}

/*
**	removeServerBlock
**	removes a server block from the server_blocks map
*/
void	BlockFinder::removeServerBlock(t_ip_str ip, t_port_str port, t_server_name server_name)
{
    std::string	key;

    mf_normalizeDirectives(ip, port, server_name);
    key = mf_hashedKey(ip, port, server_name);

    if (m_server_blocks.find(key) != m_server_blocks.end())
        m_server_blocks.erase(key);

    return;
}
