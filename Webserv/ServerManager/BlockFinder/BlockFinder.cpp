#include "BlockFinder.hpp"

BlockFinder::BlockFinder(const ServerConfig& config)
{
	_wildcard_ip = "0.0.0.0";
	_wildcard_port = "*";
	_wildcard_server_name = "*";
}

BlockFinder::~BlockFinder() {}

bool    BlockFinder::is_wildcard(const std::string &str, const std::string &wildcard)
{
    return (str.empty() || str == wildcard || str.find(wildcard) != std::string::npos);
}

std::string	BlockFinder::hashed_key(t_ip_str ip, t_port_str port, t_server_name server_name)
{
    return (std::string(ip + "|" + port + "|" + server_name));
}

void	BlockFinder::addServerBlock(const ServerBlocks& block, t_ip_str ip, t_port_str port, t_server_name server_name)
{
    // check if ip is wildcard or is empty
    if (is_wildcard(ip, _wildcard_ip)) {
        ip = _wildcard_ip;
    }

    // check if port is wildcard or is empty
    if (is_wildcard(port, _wildcard_port)) {
        port = _wildcard_port;
    }

    // check if server_name is wildcard or is empty
    if (is_wildcard(server_name, _wildcard_server_name)) {
        server_name = _wildcard_server_name;
    }

    std::string key = hashed_key(ip, port, server_name);
    server_blocks_map[key] = &block;
}

const ServerBlocks*	BlockFinder::findServerBlock(t_ip_str ip, t_port_str port, t_server_name server_name)
{
    std::string key = hashed_key(ip, port, server_name);
    return (server_blocks_map[key]);
}
