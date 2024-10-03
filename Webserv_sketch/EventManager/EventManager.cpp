/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:12:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 11:15:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "EventManager.hpp"
# include "../Event/Event.hpp"
# include "../Connection/Connection.hpp"
# include "../Globals/Globals.hpp"
# include "../FileDescriptor/FileDescriptor.hpp"

EventManager::EventManager(Globals* globals) :
    _waitCount      (0),
    _globals        (globals)
{

    assert(globals != NULL);

    _epollfd = epoll_create(1);

    if (_epollfd == -1)
    {
        _globals->_logFile->record("epoll_create(): " + std::string(strerror(errno)));
        throw std::runtime_error("epoll_create(), critical error: " + std::string(strerror(errno)));
    }
    
    if (!FileDescriptor::setCloseOnExec_NonBlocking(_epollfd))
    {
        throw std::runtime_error("setCloseOnExec(), critical error: " + std::string(strerror(errno)));
    }
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
    {
        _globals->_logFile->record("epoll_ctl(): " + std::string(strerror(errno)));
        throw std::runtime_error("epoll_ctl(), critical error: " + std::string(strerror(errno)));
    }
        
}

void                EventManager::modEvent(Event& monitor)
{
    t_epoll_event event;

    event.events = monitor._flags;
    event.data.ptr = &monitor;
    
    if (epoll_ctl(_epollfd, EPOLL_CTL_MOD, monitor._connection->_sockfd, &event) == -1)
    {
        _globals->_logFile->record("epoll_ctl(): " + std::string(strerror(errno)));
        throw std::runtime_error("epoll_ctl(), critical error: " + std::string(strerror(errno)));
    }
}

void                EventManager::delEvent(Event& monitor)
{
    if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, monitor._connection->_sockfd, NULL) == -1)
    {
        _globals->_logFile->record("epoll_ctl(): " + std::string(strerror(errno)));
        throw std::runtime_error("epoll_ctl(), critical error: " + std::string(strerror(errno)));
    }
}

int                 EventManager::waitEvents(int timeOut)
{
    return (epoll_wait(_epollfd, _events, MAX_EPOLL_EVENTS, timeOut));
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

