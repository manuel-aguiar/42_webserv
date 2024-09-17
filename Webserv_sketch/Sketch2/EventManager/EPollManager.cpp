/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EPollManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 09:57:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/17 10:47:00 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EPollManager.hpp"

EPollManager::EPollManager() :
    FileDescriptor(epoll_create(1)),
    _waitCount(0)
{
    if (_fd == -1)
        throw ParameterException("EPollManager Constructor failed", "epoll_create", std::strerror(errno));
    setCloseOnExec_NonBlocking();
    _waitCount = 0;
}

EPollManager::~EPollManager()
{
    
}

void                EPollManager::addEventFd(IEvent& monitor)
{
    if (epoll_ctl(_fd, EPOLL_CTL_ADD, monitor.getFd(), static_cast<EPollEvent*>(&monitor)) == -1)
        throw ParameterException("EPollManager addFd failed", "epoll_ctl", std::strerror(errno));
}

void                EPollManager::modEventFd(IEvent& monitor)
{
    if (epoll_ctl(_fd, EPOLL_CTL_MOD, monitor.getFd(), static_cast<EPollEvent*>(&monitor)) == -1)
        throw ParameterException("EPollManager modFd failed", "epoll_ctl", std::strerror(errno));
}

void                EPollManager::delEventFd(IEvent& monitor)
{
    if (epoll_ctl(_fd, EPOLL_CTL_DEL, monitor.getFd(), NULL) == -1)
        throw ParameterException("EPollManager delFd failed", "epoll_ctl", std::strerror(errno));
}

int                 EPollManager::waitEvents(int timeOut)
{
    _waitCount = epoll_wait(_fd, _events, MAX_EPOLL_EVENTS, timeOut);
    if (_waitCount == -1)
        throw ParameterException("EPollManager waitEvents failed", "epoll_wait", std::strerror(errno));
    return (_waitCount);
}

const   IEvent&     EPollManager::getEvent(int index)
{
    assert(index >= 0 && index < _waitCount);
    return (_events[index]);
}