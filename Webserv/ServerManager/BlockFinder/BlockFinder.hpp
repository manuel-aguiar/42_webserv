/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlockFinder.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 09:09:59 by manuel            #+#    #+#             */
/*   Updated: 2024/11/22 10:34:19 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCKFINDER_HPP

# define BLOCKFINDER_HPP

// our headers
# include "../../GenericUtils/Webserver_Definitions.h"

// C++ headers
# include <string>

class ServerConfig;
class ServerBlocks;

class BlockFinder
{
	public:
		BlockFinder(const ServerConfig& config);
		~BlockFinder();

		void			addServerBlock(const ServerBlocks& block, t_ip_str ip, t_port_str port, t_server_name server_name);
		ServerBlocks*	findServerBlock(t_ip_str ip, t_port_str port, t_server_name server_name);

		void    findBlocks();

	private:

		/*
			// here is some structure to map ip/port/host to serverblock
			// can be a hashtable or a map, or nested map.....
			// be mindfull that specific IP has precedence over wildcard IP (0.0.0.0).
			// nginx implements wildcard ports... i think we won't need that
			// first check a specific ip reference. if exists, check port. if exists, check host.

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
};

#endif
