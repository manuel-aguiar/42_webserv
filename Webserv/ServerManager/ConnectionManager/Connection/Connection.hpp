/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/28 16:02:30 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP

# define CONNECTION_HPP

# include "../../GenericUtils/Webserver_Definitions.h"

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


        Event*              m_readEvent;
        Event*              m_writeEvent;
        ListeningSocket*    m_listener;
        Nginx_MemoryPool*   m_memPool;
        Globals*            m_globals;

		void*				m_proto_connection;				// <- the http connection
		void*				m_protoModule;					// <- the http module in our case
															// set by the listening socket

        Connection(const Connection& other);
        Connection& operator=(const Connection& other);


};



#endif
