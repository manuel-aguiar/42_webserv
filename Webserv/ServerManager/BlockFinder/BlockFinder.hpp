/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlockFinder.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 09:09:59 by manuel            #+#    #+#             */
/*   Updated: 2024/11/22 09:14:05 by manuel           ###   ########.fr       */
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

		void	addServerBlock(const ServerBlocks& block, tp_ip_str ip, t_port_str port, );

		void    findBlocks();

	private:
		// here is some structure to map ip/port/host to serverblock
		// can be a hashtable or a map, or nested map.....
};

#endif
