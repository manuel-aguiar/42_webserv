/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/27 16:45:15 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP

# define CONNECTION_HPP

# include "../Webserver_Definitions.h"
# include "../ListeningSocket/ListeningSocket.hpp"
# include "../MemoryPool/MemoryPool.h"


class Connection
{
    public:
        
        static Connection* create(ListeningSocket* listener);

        void    reset()
        {
            _listener = NULL;
            _pool.reset();
        }

    private:
        t_socket            _sockfd;
        t_sockaddr*         _addr;
        t_socklen           _addrlen;

        //will be spawned via static create.()
        Connection();
        ListeningSocket*    _listener;      //pointer cause it may be reused
        Nginx_MemoryPool&   _pool;          //will have its own pool
};



#endif