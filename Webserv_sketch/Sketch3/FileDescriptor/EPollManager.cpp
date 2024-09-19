/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EPollManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 09:57:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 12:32:17 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EPollManager.hpp"

EPollManager::EPollManager() :
    _fd(epoll_create(1)),
    _waitCount(0)
{
    if (_fd == -1)
        throw ParameterException("EPollManager Constructor failed", "epoll_create", std::strerror(errno));
    _waitCount = 0;
}

EPollManager::~EPollManager()
{
    
}

const int    EPollManager::getFd() const
{
    return (_fd);
}

void                EPollManager::addEventFd(const int fd, const int eventsToMonitor)
{
    struct epoll_event newEvent = (struct epoll_event) {};
    
    newEvent.data.fd = fd;
    newEvent.events = eventsToMonitor;
    if (epoll_ctl(_fd, EPOLL_CTL_ADD, fd, &newEvent) == -1)
        throw ParameterException("EPollManager addFd failed", "epoll_ctl", std::strerror(errno));
}

void                EPollManager::modEventFd(const int fd, const int eventsToMonitor)
{
    struct epoll_event newEvent = (struct epoll_event) {};

    newEvent.data.fd = fd;
    newEvent.events = eventsToMonitor;
    if (epoll_ctl(_fd, EPOLL_CTL_MOD, fd, &newEvent) == -1)
        throw ParameterException("EPollManager modFd failed", "epoll_ctl", std::strerror(errno));
}

void                EPollManager::delEventFd(const int fd)
{
    if (epoll_ctl(_fd, EPOLL_CTL_DEL, fd, NULL) == -1)
        throw ParameterException("EPollManager delFd failed", "epoll_ctl", std::strerror(errno));
}

int                 EPollManager::waitEvents(int timeOut)
{
    _waitCount = epoll_wait(_fd, _events, MAX_EPOLL_EVENTS, timeOut);
    if (_waitCount == -1)
        throw ParameterException("EPollManager waitEvents failed", "epoll_wait", std::strerror(errno));
    return (_waitCount);
}

const   struct epoll_event&     EPollManager::getEvent(int index)
{
    assert(index >= 0 && index < _waitCount);
    return (_events[index]);
}

void        EPollManager::addEventFd(const FileDescriptor& fd, const int eventsToMonitor) { addEventFd(fd.getFd(), eventsToMonitor); }
void        EPollManager::modEventFd(const FileDescriptor& fd, const int eventsToMonitor) { modEventFd(fd.getFd(), eventsToMonitor); }
void        EPollManager::delEventFd(const FileDescriptor& fd) { delEventFd(fd.getFd()); }