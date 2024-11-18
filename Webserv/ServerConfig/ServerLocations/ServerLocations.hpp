/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLocations.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:56:34 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/18 11:00:21 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERLOCATIONS_HPP

# define SERVERLOCATIONS_HPP


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




    private:
        ServerBlock&        m_block;
        t_locationType      m_type;


};



#endif