#ifndef BLOCKFINDER_HPP
# define BLOCKFINDER_HPP

// our headers
# include "../../GenericUtils/Webserver_Definitions.h"
# include "_Tests/TestDependencies.hpp"

// C++ headers
# include <string>
# include <map>
# include <set>
# include <iostream>
# include <iomanip>


class BlockFinder {
	public:
		BlockFinder();
		~BlockFinder();

		void				addServerBlock(const ServerBlock& block);
		const ServerBlock*	findServerBlock(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName);
		bool				hasServerBlock(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName);
		void				removeServerBlock(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName);

	private:
		const std::string			m_wildcardIp;
		const std::string			m_wildcardPort;
		const std::string			m_wildcardServerName;

		// normalized values
		std::string					m_normalizedIp;
		std::string					m_normalizedPort;
		std::string					m_normalizedServerName;

		std::map<std::string, const ServerBlock*>	m_serverBlocks;

		std::string			mf_hashedKey(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName) const;
		void				mf_normalizeDirectives(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName);
		bool				mf_nonEmptyDirective(const std::string& str, const std::string& wildcard) const;
};

#endif
