/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MockEventManager.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:12:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/13 10:44:42 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../EventManager/EventManager.hpp"
# include "../../Connection/Connection.hpp"
# include "../../Event/Event.hpp"
# include "../../Globals/Globals.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

EventManager::EventManager(Globals& globals) :
	m_epollfd		(-1),
	m_waitCount		(0),
	m_globals		(globals)
{

	m_epollfd = epoll_create(1);

	if (m_epollfd == -1)
	{
		m_globals.logError("epoll_create(): " + std::string(strerror(errno)));
		throw std::runtime_error("epoll_create(), critical error: " + std::string(strerror(errno)));
	}

	if (!FileDescriptor::setCloseOnExec_NonBlocking(m_epollfd))
	{
		m_globals.logError("fcntl(): " + std::string(strerror(errno)));
		throw std::runtime_error("setCloseOnExec(), critical error: " + std::string(strerror(errno)));
	}
}

EventManager::~EventManager()
{
	if (m_epollfd != -1)
	{
		if (close(m_epollfd) == -1)
		{
			m_globals.logError("close(): " + std::string(strerror(errno)));
		}
	}
}


int                EventManager::addEvent(const Event& event)
{
	t_epoll_event epollEvent = (t_epoll_event){};

	epollEvent.events = event.getFlags();
	epollEvent.data.ptr = (void *)&event;

	if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, event.getFd(), &epollEvent) == -1)
	{
		m_globals.logError("epoll_ctl(): " + std::string(strerror(errno)));
		return (0);
	}
	return (1);
}

int                EventManager::modEvent(const Event& event)
{
	t_epoll_event epollEvent = (t_epoll_event){};

	epollEvent.events = event.getFlags();
	epollEvent.data.ptr = (void *)&event;

	if (epoll_ctl(m_epollfd, EPOLL_CTL_MOD, event.getFd(), &epollEvent) == -1)
	{
		m_globals.logError("epoll_ctl(): " + std::string(strerror(errno)));
		return (0);
	}
	return (1);
}

int                 EventManager::delEvent(const Event& event)
{
	if (epoll_ctl(m_epollfd, EPOLL_CTL_DEL, event.getFd(), NULL) == -1)
	{
		m_globals.logError("epoll_ctl(): " + std::string(strerror(errno)));
		return (0);
	}
	return (1);
}

int                 EventManager::waitEvents(int timeOut)
{
	//std::cout << "                waiting for events" << std::endl;
	m_waitCount = epoll_wait(m_epollfd, m_events, MAX_EPOLL_EVENTS, timeOut);
	//std::cout << "                         events arrived, fds: " << m_waitCount << std::endl;
	//for (int i = 0; i < m_waitCount; i++)
	//{
	// std::cout << " " << ((Event*)m_events[i].data.ptr);
	//}
	//std::cout << std::endl;
	return (m_waitCount);
}

const   t_epoll_event&     EventManager::retrieveEvent(int index)
{
	assert(index >= 0 && index < m_waitCount);
	return (m_events[index]);
}

void    EventManager::distributeEvents()
{
	Event* event;

	for (int i = 0; i < m_waitCount; i++)
	{
		event = (Event*)m_events[i].data.ptr;

		if (m_events[i].events & EPOLLIN || m_events[i].events & EPOLLOUT)
		{
			event->handle();
		}

		if (m_events[i].events & EPOLLHUP || m_events[i].events & EPOLLERR || m_events[i].events & EPOLLRDHUP)
		{

		}
	}
	
}


//private

EventManager::EventManager(const EventManager& copy) :
	m_epollfd		(copy.m_epollfd),
	m_waitCount		(copy.m_waitCount),
	m_globals		(copy.m_globals)
{}

EventManager& EventManager::operator=(const EventManager& assign)
{
	(void)assign;
	return (*this);
}