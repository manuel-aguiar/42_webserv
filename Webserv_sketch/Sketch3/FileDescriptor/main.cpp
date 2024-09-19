/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:39:49 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 16:57:35 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileDescriptorManager.hpp"
#include "EPollEvent.hpp"
#include "FileDescriptor.hpp"
#include "EPollManager.hpp"
#include "Socket.hpp"

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>

void    func(FileDescriptorManager& manager, EPollManager& poll)
{
    FileDescriptor epollfd = FileDescriptor(poll.getFd(), FileDescriptor::FD_EPOLL, true);
    FileDescriptor stdinfd = FileDescriptor(STDIN_FILENO, FileDescriptor::FD_STANDARD, false);
    FileDescriptor stdoutfd = FileDescriptor(STDOUT_FILENO, FileDescriptor::FD_STANDARD, false);
    
    manager.addFd(epollfd);

    poll.addEventFd(manager.addFd(stdinfd), EPollEvent::READ);
    poll.addEventFd(manager.addFd(stdoutfd), EPollEvent::WRITE);
}


int    setupServerSockets(FileDescriptorManager& fdManager, EPollManager& poll)
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
            Socket newServerSocket = Socket::createServerSocket(*cur);
            
            FileDescriptor newServerSocketFd = FileDescriptor(newServerSocket.getFd(), FileDescriptor::FD_SOCKET_LISTENING, true);
            poll.addEventFd(fdManager.addFd(newServerSocketFd), EPollEvent::READ);
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
    FileDescriptorManager fdManager;
    EPollManager          pollManager;

    func(fdManager, pollManager);
    setupServerSockets(fdManager, pollManager);
    int events = pollManager.waitEvents(1000);

    for (int i = 0; i < events; ++i)
    {
        const struct epoll_event& event = pollManager.getEvent(i);
        FileDescriptor* eventfd = static_cast<FileDescriptor*>(event.data.ptr);
        
        if (EPollEvent::toWrite(event))
        {
            write(eventfd->getFd(), "hey boss\n", 9);
        }
        else if (EPollEvent::toRead(event))
        {
            char buffer[1024] = {0};
            read(eventfd->getFd(), buffer, 1024);
            std::cout << buffer << std::endl;
        }
    }



    events = pollManager.waitEvents(1000);

    for (int i = 0; i < events; ++i)
    {
        const struct epoll_event& event = pollManager.getEvent(i);
        FileDescriptor* eventfd = static_cast<FileDescriptor*>(event.data.ptr);
        if (EPollEvent::toWrite(event))
        {
            write(eventfd->getFd(), "hey boss\n", 9);
        }
        else if (EPollEvent::toRead(event))
        {
            char buffer[1024] = {0};
            read(eventfd->getFd(), buffer, 1024);
            std::cout << buffer << std::endl;
        }
    }

    return (0);
}