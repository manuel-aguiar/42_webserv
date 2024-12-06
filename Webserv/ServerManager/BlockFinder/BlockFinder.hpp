/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlockFinder.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rphuyal <rphuyal@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 09:09:59 by manuel            #+#    #+#             */
/*   Updated: 2024/12/06 16:17:51 by rphuyal          ###   ########.fr       */
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
		ServerBlocks() {}
		~ServerBlocks() {}

		ServerBlocks&	operator=(const ServerBlocks& other) {
			if (this == &other)
				return (*this);

			return (*this);
		}
};

class BlockFinder {
	public:
		BlockFinder(const ServerConfig& config);
		~BlockFinder();

		bool				hasServerBlock(t_ip_str ip, t_port_str port, t_server_name server_name);
		void				addServerBlock(const ServerBlocks& block, t_ip_str ip, t_port_str port, t_server_name server_name);
		const ServerBlocks*	findServerBlock(t_ip_str ip, t_port_str port, t_server_name server_name);

	private:
		const ServerConfig&	 m_config;
		std::string			m_wildcard_ip;
		std::string			m_wildcard_port;
		std::string			m_wildcard_server_name;

		std::unordered_map<std::string, const ServerBlocks*>	m_server_blocks;

		std::string		hashedKey(t_ip_str ip, t_port_str port, t_server_name server_name);
		void			normalizeDirectives(t_ip_str &ip, t_port_str &port, t_server_name &server_name);
		bool			nonEmptyDirective(const std::string &str, const std::string &wildcard);
};

/*
	we will have, as such, two maps:

	a specific ip map, whose first level key is the IP.

	a wildcard ip map, whose first level key is a port

	so a possible solution would be something like two maps:

	1) std::map<t_ip_str, std::map<t_port_str, std::map<t_server_name, ServerBlocks* > > >
	2) std::map<t_port_str, std::map<t_server_name, ServerBlocks* > >

	We would need some default value to specify a wildcard ip..... Maybe std::string can be empty when
	inserting, or t_ip_str = "0.0.0.0", we could figure that out. Not super memory efficient but hey,
	if you want memory efficiency don't use c++98, upgrade to at least c++11 or GTFO

	So, when adding to our map structure:
		check if ip is specific -> insert at map 1
		check if ip is wildcard -> insert at map 2

	Obviously, the same entry cannot be placed in two maps simultatenously -> defeats the purpose.


	When looking up:
		check specific first-> nested find in map 1), if fails:
		check wildcard -> nested find in map 2) if fails:
			return (NULL) -> no server block was found.


	If the http interpreter is not able to find a server block, it should return a 404 error.
*/

#endif
