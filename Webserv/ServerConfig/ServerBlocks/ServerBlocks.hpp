/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlocks.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:19:34 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/20 10:18:14 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERBLOCKS_HPP

# define SERVERBLOCKS_HPP

// C++ headers
# include <string>
# include <map>
# include <set>

class ServerLocations;

class ServerBlocks
{
    public:
        //constructor destructors

        const std::set<std::string>&					getDomainName() const;
		const std::string& 								getIp() const;
		const std::string& 								getPort() const;
		const std::string& 								getRoot() const;
		const std::map<std::string, ServerLocations>& 	getLocations() const;


    private:


        std::set<std::string>                   m_ServerName;		//would eb easier to only accept one name per serverblock
		std::string							 	m_ip;
        std::string                             m_port;
        std::string                             m_root;
        std::map<std::string, ServerLocations>   m_locations;		//std::string would be the path of the location

};



#endif
