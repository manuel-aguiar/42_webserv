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
# include "../WebservDefaultConfig.h"

// C headers
# include <stddef.h>

class DefaultConfig
{
    public:
        DefaultConfig();

        //blabla all default and public, it is just a macro bearer, getters and setters are a waste;

        const size_t    maxWorkers = DEFAULTCONF_GEN_MAX_WORKERS;
        const size_t    maxClientBodySize = DEFAULTCONF_HTTP_MAX_CLIENT_BODY_SIZE;
        const size_t    maxClientHeaderSize = DEFAULTCONF_HTTP_MAX_CLIENT_HEADER_SIZE;
        const char*     IPListen = DEFAULTCONF_BLOCK_IP_LISTEN;
        const size_t    maxConnections = DEFAULTCONF_BLOCK_MAX_CONNECTIONS;
        const size_t    maxCGI = DEFAULTCONF_BLOCK_MAX_CGI;
        

        // add the remaining error pages
        const char*     http400Path = DEFAULTCONF_BLOCK_ERROR_400_PAGE;
        const char*     http404Path = DEFAULTCONF_BLOCK_ERROR_404_PAGE;
};



#endif