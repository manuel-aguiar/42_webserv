/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlockFinder.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rphuyal <rphuyal@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 09:09:59 by manuel            #+#    #+#             */
/*   Updated: 2025/01/18 14:50:03 by rphuyal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCKFINDER_HPP
# define BLOCKFINDER_HPP

// our headers
# include "../../GenericUtils/Webserver_Definitions.h"

// C++ headers
# include <string>

// TODO: remove temporary declerations
class ServerConfig {
	public:
		ServerConfig() {}
		~ServerConfig() {}
};

class ServerBlocks {
	public:
		ServerBlocks(const std::string& id) : m_id(id) {}
		~ServerBlocks() {}

		ServerBlocks&	operator=(const ServerBlocks& other) {
			if (this == &other)
				return (*this);

			return (*this);
		}

		// used for testing
		std::string		id() const { return (m_id); }

	private:
		std::string		m_id;
};
// TODO: remove temporary declerations until here


class BlockFinder {
	public:
		BlockFinder(const ServerConfig& config);
		~BlockFinder();

		void				addServerBlock(const ServerBlocks& block, const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName);
		const ServerBlocks*	findServerBlock(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName);
		bool				hasServerBlock(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName);
		void				removeServerBlock(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName);

	private:
		const ServerConfig&			 m_config;
		const std::string			m_wildcardIp;
		const std::string			m_wildcardPort;
		const std::string			m_wildcardServerName;

		// normalized values
		std::string					m_normalizedIp;
		std::string					m_normalizedPort;
		std::string					m_normalizedServerName;

		std::map<std::string, const ServerBlocks*>	m_serverBlocks;

		std::string			mf_hashedKey(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName) const;
		void				mf_normalizeDirectives(const t_ip_str& ip, const t_port_str& port, const t_server_name& serverName);
		bool				mf_nonEmptyDirective(const std::string& str, const std::string& wildcard) const;
};

#endif
