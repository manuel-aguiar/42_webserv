/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlocks.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:19:34 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/20 14:32:31 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERBLOCKS_HPP

# define SERVERBLOCKS_HPP

// C++ headers
# include <string>
# include <map>
# include <set>
# include <vector>

class ServerLocations;

class ServerBlocks
{
    public:
        //constructor destructors

		typedef std::pair<std::string, std::string>			t_listeners; // a pair of IP and PORT


        const std::set<std::string>&						getDomainNames() const;
		const std::vector<t_listeners>&						getListeners() const;
		const std::map<std::string, ServerLocations>& 		getLocations() const;


    private:

        std::set<std::string>                   			m_ServerName;		//would eb easier to only accept one name per serverblock
		std::vector<t_listeners>							m_listeners;
        std::string                             			m_root;
        std::map<std::string, ServerLocations>   			m_locations;		//std::string would be the path of the location

};



#endif
