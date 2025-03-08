/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EPollManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 09:57:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:33:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EPollManager.hpp"

EPollManager::EPollManager() :
    m_fd(epoll_create(1)),
    m_waitCount(0)
{
    if (m_fd == -1)
        throw ParameterException("EPollManager Constructor failed", "epoll_create", std::strerror(errno));
    m_waitCount = 0;
}

EPollManager::~EPollManager()
{
    
}

const int    EPollManager::getFd() const
{
    return (m_fd);
}
/*
void                EPollManager::addEventFd(const int fd, const int eventsToMonitor)
{
    struct epoll_event newEvent = (struct epoll_event) {};
    
    newEvent.data.fd = fd;
    newEvent.events = eventsToMonitor;
    if (epoll_ctl(m_fd, EPOLL_CTL_ADD, fd, &newEvent) == -1)
        throw ParameterException("EPollManager addFd failed", "epoll_ctl", std::strerror(errno));
}

void                EPollManager::modEventFd(const int fd, const int eventsToMonitor)
{
    struct epoll_event newEvent = (struct epoll_event) {};

    newEvent.data.fd = fd;
    newEvent.events = eventsToMonitor;
    if (epoll_ctl(m_fd, EPOLL_CTL_MOD, fd, &newEvent) == -1)
        throw ParameterException("EPollManager modFd failed", "epoll_ctl", std::strerror(errno));
}

void                EPollManager::delEventFd(const int fd)
{
    if (epoll_ctl(m_fd, EPOLL_CTL_DEL, fd, NULL) == -1)
        throw ParameterException("EPollManager delFd failed", "epoll_ctl", std::strerror(errno));
}

*/

int                 EPollManager::waitEvents(int timeOut)
{
    m_waitCount = epoll_wait(m_fd, m_events, MAX_EPOLL_EVENTS, timeOut);
    if (m_waitCount == -1)
        throw ParameterException("EPollManager waitEvents failed", "epoll_wait", std::strerror(errno));
    return (m_waitCount);
}

const   struct epoll_event&     EPollManager::getEvent(int index)
{
    assert(index >= 0 && index < m_waitCount);
    return (m_events[index]);
}

void        EPollManager::addEventFd(FileDescriptor* fd, const int eventsToMonitor)
{ 
    assert(fd != NULL);  
    struct epoll_event newEvent = (struct epoll_event) {};
    
    newEvent.data.ptr = fd;
    newEvent.events = eventsToMonitor;
    if (epoll_ctl(m_fd, EPOLL_CTL_ADD, fd->getFd(), &newEvent) == -1)
        throw ParameterException("EPollManager addFd failed", "epoll_ctl", std::strerror(errno));
}

void        EPollManager::modEventFd(FileDescriptor* fd, const int eventsToMonitor) 
{
    assert(fd != NULL);  
    struct epoll_event newEvent = (struct epoll_event) {};

    newEvent.data.ptr = fd;
    newEvent.events = eventsToMonitor;
    if (epoll_ctl(m_fd, EPOLL_CTL_MOD, fd->getFd(), &newEvent) == -1)
        throw ParameterException("EPollManager modFd failed", "epoll_ctl", std::strerror(errno));
}

void        EPollManager::delEventFd(FileDescriptor* fd)
{
    assert(fd != NULL);  
    if (epoll_ctl(m_fd, EPOLL_CTL_DEL, fd->getFd(), NULL) == -1)
        throw ParameterException("EPollManager delFd failed", "epoll_ctl", std::strerror(errno));
}