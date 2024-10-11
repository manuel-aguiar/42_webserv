/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/11 17:00:52 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP

# define CONNECTION_HPP

# include "../Webserver_Definitions.h"
# include "../Event/Event.hpp"
# include "../Logs/Logs.h"

class ListeningSocket;
class Event;

class Connection
{
    public:
        Connection(Globals* globals = NULL);
        ~Connection();

        void    init();

        void    reset();
        void    read();
        void    write();
        void    close();


    //private:
        t_socket            m_sockfd;
        t_sockaddr*         m_addr;
        t_socklen           m_addrlen;

        //will be spawned via static create.()

        Event*              m_readEvent;         //pointer cause it may be reused
        Event*              m_writeEvent;
        ListeningSocket*    m_listener;          //pointer cause it may be reused
        Nginx_MemoryPool*   m_memPool;           //will have its own pool
        Globals*            m_globals;

        Connection(const Connection& other);
        Connection& operator=(const Connection& other);


};



#endif
