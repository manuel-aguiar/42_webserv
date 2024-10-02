/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:03:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/02 08:05:11 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP

# define SERVER_HPP

# include "../Webserver_Definitions.h"
# include "../CgiHandler/CgiHandler.hpp"
# include "../ListeningSocket/ListeningSocket.hpp"
# include "../Connection/ConnectionPool.hpp"
# include "../Logs/Logs.h"

# include <vector>
# include <map>
# include <iostream>

class Globals;

class Server
{
    public:
        Server(Globals* globals);
        ~Server();
        
        int createListeners(const char* node, const char* port, int socktype, int ai_family, int backlog); 
    
    //private:
        //typedef MemoryPool_Alloc<std::pair<int, ListeningSocket> > ListeningSocketPool;
        //typedef std::map<int, ListeningSocket, std::less<int>, MemoryPool_Alloc<std::pair<int, ListeningSocket> > > ListeningSocketMap;
        //ListeningSocketMap              _listeners;
        
        size_t                          _myID;
        bool                            _multithreadListen;
        std::vector<ListeningSocket*>   _listeners;
        Nginx_MemoryPool*               _pool;
        Cgi_Handler                     _cgiHandler;
        ConnectionPool                  _connectionPool;
        Globals*                        _globals;
};





#endif