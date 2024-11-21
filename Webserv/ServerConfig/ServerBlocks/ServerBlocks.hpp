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

// Own headers
# include "../../GenericUtils/Webserver_Definitions.h"

class ServerLocations;

class ServerBlocks
{
    public:
        //constructor destructors

        const std::set<std::string>&						getDomainNames() const;
		const std::vector<t_listeners>&						getListeners() const;
		const std::map<std::string, ServerLocations>& 		getLocations() const;


    private:

        std::set<std::string>                   			m_ServerName;		// would be easier to only accept one name per serverblock
		std::vector<t_listeners>							m_listeners;
        std::string                             			m_root;
        std::map<t_path, ServerLocations>   			    m_locations;

};



#endif
