/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestServerConfig.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:50:49 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/12 14:47:06 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP

# define SERVERCONFIG_HPP

// C++ Headers
# include <cstddef>
# include <set>
# include <string>
# include <map>

//forward declarations
class DefaultConfig;
class ServerBlock;
class Globals;

class ServerConfig
{
    public:
        ServerConfig(const char* configFile, const DefaultConfig& configDefault, Globals* globals);

		const std::map<std::string, ServerBlock>&			getServerBlocks() const;
		size_t 												getNumWorkers() const;
		size_t 												getMaxConnections() const;
		size_t 												getMaxCgi() const;

    private:
        ServerConfig();

		std::map<std::string, ServerBlock>		m_serverBlocks;		// std::string would be like IP:PORT:server_name as the identifier...?
		size_t									m_maxWorkers;
		size_t									m_maxConnections;
		size_t									m_maxCgi;


		// mostly for logs and debuging, see Globals class
		Globals*								m_globals;
};




#endif
