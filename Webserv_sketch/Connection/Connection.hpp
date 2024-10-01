/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/01 19:48:00 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP

# define CONNECTION_HPP

# include "../Webserver_Definitions.h"
# include "../MemoryPool/MemoryPool.h"
# include "../Event/Event.hpp"
# include "../Logs/Logs.h"

class ListeningSocket;
class Event;

class Connection
{
    public:
        Connection(ILog* logFile = NULL);
        ~Connection();

        void    init();

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
        Nginx_MemoryPool*   _connectionAlloc;          //will have its own pool
        ILog*               _logFile;
        
        Connection(const Connection& other);
        Connection& operator=(const Connection& other);


};



#endif