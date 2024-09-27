/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:03:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/27 15:34:37 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() :
    _pool(Nginx_MemoryPool::create(1024, 5))
{
    
}


/*


int setupListeners()
{

    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *cur;

    hints = (struct addrinfo){};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo(NULL, "8080", &hints, &res);
	if (status != 0)
	{
		std::cerr << "getaddrinfo(): " + std::string(gai_strerror(status)) << std::endl;
		return (1);
	}
    
    for(cur = res; cur != NULL; cur = cur->ai_next)
	{
        try
        {
            UniquePtr<IServerSocket> serverSock = ServerSocketFactory::create(*cur);
            if (!serverSock.get())
            {
                std::cerr << "server not added" << std::endl;
                continue ;
            }
            serverSock->bind();
            serverSock->listen();
            serverSock->setFdManager(&fdManager);
            fdManager.addFd(serverSock, true);
            std::cout << "server added" << std::endl;   
        }
        catch(const std::exception& e)
        {
            //alloc failed, non critical...?
            std::cerr << e.what() << '\n';
        }
    }   
    freeaddrinfo(res);
    return (0);
}

int main(void)
{
    setupListeners();
    return (0);
}

int main4(void)
{
    IPv4Address addr(0, 8080);
    UniquePtr<IServerSocket> server = new ServerSocket<IPv4Address>(addr, SOCK_STREAM, IPPROTO_TCP);    
    if (!server.get())
        std::cerr << "ServerSocketFactory::create() failed" << std::endl;
    return (0);
}









*/
