/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EPollEvent.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 09:00:12 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/17 10:44:15 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "EPollEvent.hpp"

EPollEvent::EPollEvent()
{
    this->data.ptr = NULL;
    this->events = 0;
}

EPollEvent::EPollEvent(IPollableFd* fdToMonitor, const int eventsToMonitor)
{
    this->data.ptr = fdToMonitor;
    this->events = eventsToMonitor;
}

EPollEvent::EPollEvent(const EPollEvent& copy)
{
    *this = copy;
}

EPollEvent& EPollEvent::operator=(const EPollEvent& assign)
{
    if (this == &assign)
        return (*this);
    this->data.ptr = assign.data.ptr;
    this->events = assign.events;
    return (*this);
}

EPollEvent::~EPollEvent()
{
    
}

EPollEvent::EPollEvent(const struct epoll_event& copy)
{
    *this = copy;
}

EPollEvent& EPollEvent::operator=(const struct epoll_event& assign)
{
    this->data.ptr = assign.data.ptr;
    this->events = assign.events;
    return (*this);
}

bool    EPollEvent::operator==(const struct epoll_event& other) const
{
    return (this->data.ptr == other.data.ptr && this->events == other.events);
}

bool    EPollEvent::toClose() const
{
    return (this->events & EPOLLHUP);
}

bool    EPollEvent::toRead() const
{
    return (this->events & EPOLLIN);
}

bool    EPollEvent::toWrite() const
{
    return (this->events & EPOLLOUT);
}

bool    EPollEvent::toError() const
{
    return (this->events & EPOLLERR);
}

int     EPollEvent::getFd() const
{
    return ((static_cast<IPollableFd*>(this->data.ptr))->getFd());
}