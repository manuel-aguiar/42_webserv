/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/21 11:03:00 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP

# define CONNECTION_HPP

# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../EventManager/Event/Event.hpp"
# include "../../../Globals/LogFile/LogFile.hpp"

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

        Connection(const Connection& other);
        Connection& operator=(const Connection& other);


};



#endif
