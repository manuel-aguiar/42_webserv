/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:50:49 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/20 10:13:49 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP

# define SERVERCONFIG_HPP

// C++ Headers
# include <cstddef>
# include <set>
# include <string>
# include <map>

class DefaultConfig;
class ServerBlocks;
class Globals;

class ServerConfig
{
    public:
        ServerConfig(const char* configFile, const DefaultConfig& configDefault, Globals* globals);

		const std::map<std::string, ServerBlocks>&			getServerBlocks() const;
		size_t 												getMaxWorkers() const;
		size_t 												getMaxConnections() const;
		size_t 												getMaxCgi() const;

    private:
        ServerConfig();

		std::map<std::string, ServerBlocks>		m_serverBlocks;		// std::string would be like IP:PORT:server_name as the identifier...?
		size_t									m_maxWorkers;
		size_t									m_maxConnections;
		size_t									m_maxCgi;


};




#endif
