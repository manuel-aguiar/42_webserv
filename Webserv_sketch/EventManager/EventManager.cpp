/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:12:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:33:24 by mmaria-d         ###   ########.fr       */
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
    m_waitCount      (0),
    m_globals        (globals)
{

    assert(globals != NULL);

    m_epollfd = epoll_create(1);

    if (m_epollfd == -1)
    {
        m_globals->m_logFile->record("epoll_create(): " + std::string(strerror(errno)));
        throw std::runtime_error("epoll_create(), critical error: " + std::string(strerror(errno)));
    }
    
    if (!FileDescriptor::setCloseOnExec_NonBlocking(m_epollfd))
    {
        m_globals->m_logFile->record("fcntl(): " + std::string(strerror(errno)));
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

    event.events = monitor.m_flags;
    event.data.ptr = &monitor;
    
    if (monitor.m_fd == 0)
    {
        std::cout << "subscribing 0, fd? " << fd << std::endl; throw std::runtime_error("subscribing 0, fd? ");
        m_globals->m_logFile->record("EventManager::addEvent(): fd == 0");
        return (0);
    }

    if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &event) == -1)
    {
        m_globals->m_logFile->record("epoll_ctl(): " + std::string(strerror(errno)));
        return (0);
    }
    return (1);        
}

int                EventManager::modEvent(t_fd fd, Event& monitor)
{
    t_epoll_event event = (t_epoll_event){};

    event.events = monitor.m_flags;
    event.data.ptr = &monitor;
    
    if (epoll_ctl(m_epollfd, EPOLL_CTL_MOD, fd, &event) == -1)
    {
        m_globals->m_logFile->record("epoll_ctl(): " + std::string(strerror(errno)));
        return (0);
    }
    return (1); 
}

int                 EventManager::delEvent(t_fd fd)
{
    if (epoll_ctl(m_epollfd, EPOLL_CTL_DEL, fd, NULL) == -1)
    {
        m_globals->m_logFile->record("epoll_ctl(): " + std::string(strerror(errno)));
        return (0);
    }
    return (1); 
}

int                 EventManager::waitEvents(int timeOut)
{   
    std::cout << "                waiting for events" << std::endl; 
    m_waitCount = epoll_wait(m_epollfd, m_events, MAX_EPOLL_EVENTS, timeOut);
     std::cout << "                         events arrived, fds: " << m_waitCount << std::endl;
     for (int i = 0; i < m_waitCount; i++)
     {
         std::cout << " " << ((Event*)m_events[i].data.ptr)->m_fd;
     }
     std::cout << std::endl;
    return (m_waitCount);
}

const   t_epoll_event&     EventManager::getEvent(int index)
{
    assert(index >= 0 && index < m_waitCount);
    return (m_events[index]);
}

#include <cstdio>

void    EventManager::distributeEvents()
{
    Event* event;
    
    for (int i = 0; i < m_waitCount; i++)
    {
        event = (Event*)m_events[i].data.ptr;

        if (m_events[i].events & EPOLLIN || m_events[i].events & EPOLLOUT)
        {
            std::cout << "              handling event " << event->m_fd << std::endl;
            event->handle();
        }
            
        if (m_events[i].events & EPOLLHUP || m_events[i].events & EPOLLERR || m_events[i].events & EPOLLRDHUP)
        {
            std::cout << "              deleting event " << event->m_fd << std::endl;
            Connection* connection = (Connection*)event->m_data;
            if (connection->m_sockfd == -1)
                continue ;
            delEvent(connection->m_sockfd);
            ::close(connection->m_sockfd);
            std::cout <<"       returning connection via epoll" << std::endl;
            connection->m_listener->m_connectionPool.returnConnection(connection);
            connection->reset();
        }
    }
}

