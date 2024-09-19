/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:39:49 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 12:33:04 by mmaria-d         ###   ########.fr       */
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

    fdManager.addFd(FileDescriptor(pollManager.getFd(), FileDescriptor::FD_EPOLL, true));
    fdManager.addFd(FileDescriptor(STDIN_FILENO, FileDescriptor::FD_STANDARD, false));
    fdManager.addFd(FileDescriptor(STDOUT_FILENO, FileDescriptor::FD_STANDARD, false));

    pollManager.addEventFd(STDIN_FILENO, EPollEvent::READ);
    pollManager.addEventFd(STDOUT_FILENO, EPollEvent::WRITE);

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