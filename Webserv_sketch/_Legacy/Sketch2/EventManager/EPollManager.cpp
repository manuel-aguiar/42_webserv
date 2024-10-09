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
    m_waitCount(0)
{
    m_fd = epoll_create(1);
    if (m_fd == -1)
        throw ParameterException("EPollManager Constructor failed", "epoll_create", std::strerror(errno));
    setCloseOnExec_NonBlocking();
    m_waitCount = 0;
}

EPollManager::~EPollManager()
{
    
}

void                EPollManager::addEventFd(IEvent& monitor)
{
    if (epoll_ctl(m_fd, EPOLL_CTL_ADD, monitor.getFd(), static_cast<EPollEvent*>(&monitor)) == -1)
        throw ParameterException("EPollManager addFd failed", "epoll_ctl", std::strerror(errno));
}

void                EPollManager::modEventFd(IEvent& monitor)
{
    if (epoll_ctl(m_fd, EPOLL_CTL_MOD, monitor.getFd(), static_cast<EPollEvent*>(&monitor)) == -1)
        throw ParameterException("EPollManager modFd failed", "epoll_ctl", std::strerror(errno));
}

void                EPollManager::delEventFd(IEvent& monitor)
{
    if (epoll_ctl(m_fd, EPOLL_CTL_DEL, monitor.getFd(), NULL) == -1)
        throw ParameterException("EPollManager delFd failed", "epoll_ctl", std::strerror(errno));
}

int                 EPollManager::waitEvents(int timeOut)
{
    m_waitCount = epoll_wait(m_fd, m_events, MAX_EPOLL_EVENTS, timeOut);
    if (m_waitCount == -1)
        throw ParameterException("EPollManager waitEvents failed", "epoll_wait", std::strerror(errno));
    return (m_waitCount);
}

const   IEvent&     EPollManager::getEvent(int index)
{
    assert(index >= 0 && index < m_waitCount);
    return (m_events[index]);
}