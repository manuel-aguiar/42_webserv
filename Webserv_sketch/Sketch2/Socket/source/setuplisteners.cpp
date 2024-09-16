/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setuplisteners.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 07:50:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 12:11:42 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
   clear && c++ -Wall -Wextra -Werror $(find . -name "*.cpp") ../FileDescriptor/FileDescriptor.cpp ../FileDescriptorManager/FileDescriptorManager.cpp ../Exceptions/ParameterException.cpp ../EventPoll/EventManager.cpp -o sockets

    valgrind --track-fds=yes ./sockets
*/

# include "../WebserverSockets.hpp"

/*

    struct addrinfo {
        int              ai_flags;
        int              ai_family;
        int              ai_socktype;
        int              ai_protocol;
        socklen_t        ai_addrlen;
        struct sockaddr *ai_addr;
        char            *ai_canonname;
        struct addrinfo *ai_next;
    };
*/

int setupListeners()
{
    FileDescriptorManager fdManager;
    
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *cur;

    hints = (struct addrinfo){};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo(NULL, "8080", &hints, &res);
	if (status != 0)
	{
		std::cerr << "getaddrinfo(): " + std::string(gai_strerror(status)) << std::endl;
		return (1);
	}
    
    for(cur = res; cur != NULL; cur = cur->ai_next)
	{
        std::cout << "loop" << std::endl;
        try
        {
            UniquePtr<IServerSocket> server = ServerSocketFactory::create(*cur);
            if (!server.get())
            {
                std::cerr << "ServerSocketFactory::create() failed" << std::endl;
                continue ;
            }
            server->bind();
            server->listen();
            server->setFdManager(&fdManager);
            fdManager.addFileDescriptor(server, true);
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


