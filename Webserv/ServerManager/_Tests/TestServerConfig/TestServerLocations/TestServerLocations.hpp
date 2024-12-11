/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLocations.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:56:34 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/20 10:18:01 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERLOCATIONS_HPP

# define SERVERLOCATIONS_HPP

#include <string>
#include <set>

class ServerBlock;

class ServerLocations
{
    public:

        typedef enum e_locationType
        {
            E_LOCATION_REGULAR,
            E_LOCATION_REDIRECTION,
            E_LOCATION_CGI
        }   t_locationType;

        ServerLocations(ServerBlock& block, t_locationType type);    //non-const, very likely location will have to update the block...?

		const ServerBlock& 			getServerBlock() const;
		t_locationType 				getLocationType() const;
		const std::string&			getPath() const;
		const std::string&			getRoot() const;

    private:
        ServerBlock&        		m_block;
        t_locationType      		m_type;

		std::string         		m_path;
		std::string					m_root;
		std::set<std::string> 		m_methods;

		// some cgi stuff with path and extension here

		// some redirection stuff with URL to follow here

		bool						m_autoindex = false;


};



#endif
