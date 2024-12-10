/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultConfig.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:20:23 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/19 13:09:23 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFAULTCONFIG_HPP

# define DEFAULTCONFIG_HPP

// Project Headers
# include "WebservDefaultConfig.h"

// C headers
# include <stddef.h>
# include <string>

class DefaultConfig
{
	public:
		DefaultConfig();

		//blabla all default and public, it is just a macro bearer, getters and setters are a waste;

		// Program
		std::string	maxConnections;
		std::string	maxCGI;

		// Server
		std::string	maxClientBodySize;
		std::string	maxClientHeaderSize;
		std::string	IPListen;
		std::string	http400Path;
		std::string	http404Path; // add the remaining error pages

		// Location
		std::string autoIndex;
		std::string	methods;
		std::string	type;

};

#endif
