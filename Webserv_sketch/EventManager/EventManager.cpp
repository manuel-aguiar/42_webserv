/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:12:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/30 15:10:59 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventManager.hpp"
#include "../Event/Event.hpp"
#include "../Connection/Connection.hpp"

EventManager::EventManager() :
    _epollfd(epoll_create(1)),
    _waitCount(0)
{
    if (_epollfd == -1)
        throw std::runtime_error("epoll_create(), critical error: " + std::string(strerror(errno)));
}

EventManager::~EventManager()
{
    
}



//private

EventManager::EventManager(const EventManager& copy)
{
    (void)copy;
}

EventManager& EventManager::operator=(const EventManager& assign)
{
    (void)assign;
    return (*this);
}





void                EventManager::addEvent(Event& monitor)
{
    t_epoll_event event;

    event.events = monitor._flags;
    event.data.ptr = &monitor;
    
    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, monitor._connection->_sockfd, &event) == -1)
        throw std::runtime_error("epoll_create(), critical error: " + std::string(strerror(errno)));
}

void                EventManager::modEvent(Event& monitor)
{
    t_epoll_event event;

    event.events = monitor._flags;
    event.data.ptr = &monitor;
    
    if (epoll_ctl(_epollfd, EPOLL_CTL_MOD, monitor._connection->_sockfd, &event) == -1)
        throw std::runtime_error("epoll_create(), critical error: " + std::string(strerror(errno)));
}

void                EventManager::delEvent(Event& monitor)
{
    if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, monitor._connection->_sockfd, NULL) == -1)
        throw std::runtime_error("epoll_create(), critical error: " + std::string(strerror(errno)));
}

int                 EventManager::waitEvents(int timeOut)
{
    _waitCount = epoll_wait(_epollfd, _events, MAX_EPOLL_EVENTS, timeOut);
    if (_waitCount == -1)
        throw std::runtime_error("epoll_create(), critical error: " + std::string(strerror(errno)));
    return (_waitCount);
}

const   t_epoll_event&     EventManager::getEvent(int index)
{
    assert(index >= 0 && index < _waitCount);
    return (_events[index]);
}

void    EventManager::distributeEvents()
{
    for (int i = 0; i < _waitCount; i++)
    {
        Event* event = (Event*)_events[i].data.ptr;
        event->handle();
    }
}

