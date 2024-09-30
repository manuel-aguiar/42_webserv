/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:03:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/30 10:18:11 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP

# define SERVER_HPP

# include "../Webserver_Definitions.h"
# include "../CgiHandler/CgiHandler.hpp"
# include "../ListeningSocket/ListeningSocket.hpp"
# include <vector>
# include <map>
# include <iostream>

class Server
{
    public:
        Server();
        ~Server();
        
        int createListeners(const char* node, const char* port, int socktype, int ai_family, int backlog); 
    
    //private:
        //typedef MemoryPool_Alloc<std::pair<int, ListeningSocket> > ListeningSocketPool;
        //typedef std::map<int, ListeningSocket, std::less<int>, MemoryPool_Alloc<std::pair<int, ListeningSocket> > > ListeningSocketMap;
        //ListeningSocketMap              _listeners;
        
        bool                            _multithreadListen;
        
        std::vector<ListeningSocket*>    _listeners;

        Nginx_MemoryPool*               _pool;

        Cgi_Handler                     _cgiHandler;
};





#endif