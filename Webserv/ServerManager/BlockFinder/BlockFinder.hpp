/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlockFinder.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 09:09:59 by manuel            #+#    #+#             */
/*   Updated: 2024/12/10 10:10:16 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCKFINDER_HPP
# define BLOCKFINDER_HPP

// our headers
# include "../../GenericUtils/Webserver_Definitions.h"

// C++ headers
# include <string>

//forward declarations
class ServerConfig;
class ServerBlock;

class BlockFinder {
	public:
		BlockFinder(const ServerConfig& config);
		~BlockFinder();

		void				addServerBlock(const ServerBlock& block, const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName);
		const ServerBlock*	findServerBlock(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName);
		bool				hasServerBlock(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName);
		void				removeServerBlock(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName);

	private:
		const ServerConfig&			m_config;
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
