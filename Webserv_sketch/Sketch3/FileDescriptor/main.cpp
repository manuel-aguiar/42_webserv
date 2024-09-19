/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:39:49 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 11:23:49 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileDescriptorManager.hpp"
#include "EPollEvent.hpp"
#include "FileDescriptor.hpp"
#include "EPollManager.hpp"

#include <iostream>

int main(void)
{
    FileDescriptorManager fdManager;
    EPollManager          pollManager;
    FileDescriptor        stdinFd(STDIN_FILENO, false);
    FileDescriptor        stdoutFd(STDOUT_FILENO, false);

    fdManager.addFd(pollManager.getFd());
    fdManager.addFd(stdinFd);
    fdManager.addFd(stdoutFd);

    pollManager.addEventFd(stdinFd, EPollEvent::READ);
    pollManager.addEventFd(stdoutFd, EPollEvent::WRITE);

    int events = pollManager.waitEvents(1000);

    for (int i = 0; i < events; ++i)
    {
        const struct epoll_event& event = pollManager.getEvent(i);

        if (EPollEvent::toWrite(event))
        {
            write(event.data.fd, "hey boss\n", 9);
        }
        else if (EPollEvent::toRead(event))
        {
            char buffer[1024] = {0};
            read(event.data.fd, buffer, 1024);
            std::cout << buffer << std::endl;
        }
    }



    events = pollManager.waitEvents(1000);

    for (int i = 0; i < events; ++i)
    {
        const struct epoll_event& event = pollManager.getEvent(i);

        if (EPollEvent::toWrite(event))
        {
            write(event.data.fd, "hey boss\n", 9);
        }
        else if (EPollEvent::toRead(event))
        {
            char buffer[1024] = {0};
            read(event.data.fd, buffer, 1024);
            std::cout << buffer << std::endl;
        }
    }

    return (0);
}