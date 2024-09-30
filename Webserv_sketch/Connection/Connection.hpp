/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/30 15:05:05 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP

# define CONNECTION_HPP

# include "../Webserver_Definitions.h"
# include "../MemoryPool/MemoryPool.h"
# include "../Event/Event.hpp"

class ListeningSocket;
class Event;

class Connection
{
    public:
        Connection();
        ~Connection();

        void    reset();
        void    read();
        void    write();
        void    close();


    //private:
        t_socket            _sockfd;
        t_sockaddr*         _addr;
        t_socklen           _addrlen;

        //will be spawned via static create.()
        
        Event*              _readEvent;         //pointer cause it may be reused
        Event*              _writeEvent;
        ListeningSocket*    _listener;      //pointer cause it may be reused
        Nginx_MemoryPool&   _connectionAlloc;          //will have its own pool
    


        Connection(const Connection& other);
        Connection& operator=(const Connection& other);


};



#endif