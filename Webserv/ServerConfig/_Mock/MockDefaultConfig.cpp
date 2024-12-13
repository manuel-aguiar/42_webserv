/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:50:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/13 10:50:54 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../DefaultConfig/DefaultConfig.hpp"

DefaultConfig::DefaultConfig()
{
	maxClientBodySize = DEFAULTCONF_HTTP_MAX_CLIENT_BODY_SIZE;
	maxClientHeaderSize = DEFAULTCONF_HTTP_MAX_CLIENT_HEADER_SIZE;
	IPListen = DEFAULTCONF_BLOCK_IP_LISTEN;
	maxConnections = DEFAULTCONF_BLOCK_MAX_CONNECTIONS;
	maxCGI = DEFAULTCONF_BLOCK_MAX_CGI;

	http400Path = DEFAULTCONF_BLOCK_ERROR_400_PAGE;
	http404Path = DEFAULTCONF_BLOCK_ERROR_404_PAGE;

	autoIndex = DEFAULTCONF_LOCATION_AUTOINDEX;
	methods = DEFAULTCONF_LOCATION_METHODS;
	type = DEFAULTCONF_LOCATION_TYPE;
}