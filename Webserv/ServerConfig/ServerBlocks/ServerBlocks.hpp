/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlocks.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:19:34 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/18 10:56:16 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERBLOCKS_HPP

# define SERVERBLOCKS_HPP

// C++ headers
# include <string>
# include <set>

class ServerLocation;

class ServerBlocks
{
    public:
        //constructor destructors

    private:
        std::string                             m_domainName;
        std::string                             m_port;
        std::string                             m_root;
        std::set<std::string, ServerLocation>   m_locations;

};



#endif
