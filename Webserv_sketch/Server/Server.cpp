/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:03:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/30 10:58:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() :
    _pool(Nginx_MemoryPool::create(4096, 1))
{
    #ifdef SO_REUSEPORT
        _multithreadListen = true;
    #else
        _multithreadListen = false;
    #endif
}

Server::~Server()
{
    _pool->destroy();
}

int Server::createListeners(const char* node, const char* port, int socktype, int addrFamily, int backlog)
{
    ListeningSocket*    listener;
    t_addrinfo          hints;
    t_addrinfo          *res;
    t_addrinfo          *cur;

    hints = (struct addrinfo){};
    hints.ai_family = addrFamily;
    hints.ai_socktype = socktype;

	int status = getaddrinfo(node, port, &hints, &res);
    
	if (status != 0)
	{
		std::cerr << "getaddrinfo(): " + std::string(gai_strerror(status)) << std::endl;
		return (1);
	}
    
    for(cur = res; cur != NULL; cur = cur->ai_next)
	{
        listener = (ListeningSocket *)_pool->allocate(sizeof(ListeningSocket), true);
        new (listener) ListeningSocket(_connectionPool);
        
        listener->_addr = (t_sockaddr *)_pool->allocate(cur->ai_addrlen, true);
        std::memcpy(listener->_addr, cur->ai_addr, cur->ai_addrlen);

        listener->_socktype = cur->ai_socktype;
        listener->_proto = cur->ai_protocol;
        listener->_addrlen = cur->ai_addrlen;
        listener->_backlog = backlog;
        listener->open();
        _listeners.push_back(listener);

    }   
    freeaddrinfo(res);
    return (0);
}


