/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlockFinder.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rphuyal <rphuyal@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 09:09:59 by manuel            #+#    #+#             */
/*   Updated: 2024/12/06 21:04:33 by rphuyal          ###   ########.fr       */
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

		bool				hasServerBlock(t_ip_str ip, t_port_str port, t_server_name server_name);
		void				addServerBlock(const ServerBlocks& block, t_ip_str ip, t_port_str port, t_server_name server_name);
		void				removeServerBlock(t_ip_str ip, t_port_str port, t_server_name server_name);
		const ServerBlocks*	findServerBlock(t_ip_str ip, t_port_str port, t_server_name server_name);

	private:
		const ServerConfig&	 m_config;
		std::string			m_wildcard_ip;
		std::string			m_wildcard_port;
		std::string			m_wildcard_server_name;

		std::map<std::string, const ServerBlocks*>	m_server_blocks;

		std::string			mf_hashedKey(t_ip_str ip, t_port_str port, t_server_name server_name);
		void				mf_normalizeDirectives(t_ip_str &ip, t_port_str &port, t_server_name &server_name);
		bool				mf_nonEmptyDirective(const std::string &str, const std::string &wildcard);
};

#endif
