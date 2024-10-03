/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:12:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 18:13:51 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "EventManager.hpp"
# include "../Event/Event.hpp"
# include "../Connection/Connection.hpp"
# include "../Globals/Globals.hpp"
# include "../FileDescriptor/FileDescriptor.hpp"
# include "../ListeningSocket/ListeningSocket.hpp"
# include "../Connection/ConnectionPool.hpp"

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
        _globals->_logFile->record("fcntl(): " + std::string(strerror(errno)));
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


int                EventManager::addEvent(t_fd fd, Event& monitor)
{
    t_epoll_event event = (t_epoll_event){};

    event.events = monitor._flags;
    event.data.ptr = &monitor;
    
    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &event) == -1)
    {
        _globals->_logFile->record("epoll_ctl(): " + std::string(strerror(errno)));
        return (0);
    }
    return (1);        
}

int                EventManager::modEvent(t_fd fd, Event& monitor)
{
    t_epoll_event event = (t_epoll_event){};

    event.events = monitor._flags;
    event.data.ptr = &monitor;
    
    if (epoll_ctl(_epollfd, EPOLL_CTL_MOD, fd, &event) == -1)
    {
        _globals->_logFile->record("epoll_ctl(): " + std::string(strerror(errno)));
        return (0);
    }
    return (1); 
}

int                 EventManager::delEvent(t_fd fd)
{
    if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, NULL) == -1)
    {
        _globals->_logFile->record("epoll_ctl(): " + std::string(strerror(errno)));
        return (0);
    }
    return (1); 
}

int                 EventManager::waitEvents(int timeOut)
{   
    std::cout << "                waiting for events" << std::endl; 
    _waitCount = epoll_wait(_epollfd, _events, MAX_EPOLL_EVENTS, timeOut);
     std::cout << "                         events arrived: " << _waitCount << std::endl; 
    return (_waitCount);
}

const   t_epoll_event&     EventManager::getEvent(int index)
{
    assert(index >= 0 && index < _waitCount);
    return (_events[index]);
}

#include <cstdio>

void    EventManager::distributeEvents()
{
    Event* event;
    
    for (int i = 0; i < _waitCount; i++)
    {
        event = (Event*)_events[i].data.ptr;

        if (_events[i].events & EPOLLIN || _events[i].events & EPOLLOUT)
            event->handle();
        if (_events[i].events & EPOLLHUP || _events[i].events & EPOLLERR || _events[i].events & EPOLLRDHUP)
        {
            Connection* connection = (Connection*)event->_data;
            if (connection->_sockfd == -1)
                continue ;
            delEvent(connection->_sockfd);
            ::close(connection->_sockfd);
            std::cout <<"       returning connection via epoll" << std::endl;
            connection->_listener->_connectionPool.returnConnection(connection);
            connection->reset();
        }
    }
}

