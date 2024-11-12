/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:03:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/12 10:35:38 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP

# define SERVER_HPP

# include "../Webserver_Definitions.h"
# include "../CgiHandler/CgiHandler.hpp"
# include "../ListeningSocket/ListeningSocket.hpp"
# include "../Connection/ConnectionPool.hpp"
# include "../Logs/Logs.h"
# include "../EventManager/EventManager.hpp"
# include "../Event/HandlerFunction.hpp"
# include "../SignalHandler/SignalHandler.hpp"

# include <vector>
# include <map>
# include <iostream>

class Globals;

class Server
{
    public:
        Server(size_t serverID, Globals* globals);
        ~Server();

        int createListeners(const char* node, const char* port, int socktype, int ai_family, int backlog);

        int setup_mySignalHandler();

        int run();

    //private:
        //typedef MemoryPool_Alloc<std::pair<int, ListeningSocket> > ListeningSocketPool;
        //typedef std::map<int, ListeningSocket, std::less<int>, MemoryPool_Alloc<std::pair<int, ListeningSocket> > > ListeningSocketMap;
        //ListeningSocketMap              m_listeners;

        size_t                          m_myID;
        bool                            m_multithreadListen;
        std::vector<ListeningSocket*>   m_listeners;
        Nginx_MemoryPool*               m_pool;
        CgiManager                     m_cgiHandler;
        ConnectionPool                  m_connectionPool;
        EventManager                    m_eventManager;
        Globals*                        m_globals;
        Event                           m_mySignalEvent;

        bool                            m_isRunning;



    private:
        Server();
        Server(const Server& copy);
        Server& operator=(const Server& assign);
};





#endif
