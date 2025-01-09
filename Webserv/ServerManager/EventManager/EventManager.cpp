/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:12:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/09 17:10:20 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "EventManager.hpp"
# include "../../Event/Event.hpp"
# include "../../Globals/Globals.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

EventManager::EventManager(Globals& globals) :
	m_subscribeCount(0),
	m_epollfd		(-1),
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

#include <iostream>

int                EventManager::addEvent(const Event& event)
{
	t_epoll_event epollEvent = (t_epoll_event){};
	//std::cout << "EventManager::addEvent fd: " << event.getFd() << std::endl;
	epollEvent.events = event.getMonitoredFlags();
	epollEvent.data.ptr = (void *)&event;

	if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, event.getFd(), &epollEvent) == -1)
	{
		m_globals.logError("EventManager::delEvent, epoll_ctl(): " + std::string(strerror(errno)));
		return (0);
	}
	++m_subscribeCount;

	//std::cout << "EventManager::addEvent fd: " << event.getFd() << ", subscribe count is " << m_subscribeCount << std::endl;

	return (1);
}

int                EventManager::modEvent(const Event& event)
{
	t_epoll_event epollEvent = (t_epoll_event){};

	epollEvent.events = event.getMonitoredFlags();
	epollEvent.data.ptr = (void *)&event;

	if (epoll_ctl(m_epollfd, EPOLL_CTL_MOD, event.getFd(), &epollEvent) == -1)
	{
		m_globals.logError("EventManager::delEvent, epoll_ctl(): " + std::string(strerror(errno)));
		return (0);
	}
	return (1);
}

int                 EventManager::delEvent(const Event& event)
{
	//std::cout << "EventManager::delEvent fd: " << event.getFd(); 

	if (epoll_ctl(m_epollfd, EPOLL_CTL_DEL, event.getFd(), NULL) == -1)
	{
		m_globals.logError("EventManager::delEvent, epoll_ctl(): " + std::string(strerror(errno)));

		//std::cout << " FAILED: " << std::string(strerror(errno)) << std::endl;

		return (0);
	}
	--m_subscribeCount;

	//std::cout << " passed, subscribe count is " << m_subscribeCount << std::endl;

	return (1);
}

int                 EventManager::ProcessEvents(int timeOut)
{
	Event*	event;
	int		waitCount;

	waitCount = epoll_wait(m_epollfd, m_events, MAX_EPOLL_EVENTS, timeOut);

	if (waitCount == -1)
	{
		m_globals.logError("EventManager::ProcessEvents, epoll_wait(): " + std::string(strerror(errno)));
		return (waitCount);
	}
	for (int i = 0; i < waitCount; i++)
	{
		event = static_cast<Event*>(m_events[i].data.ptr);
		event->setTriggeredFlags(m_events[i].events);
		event->handle();
	}
	return (waitCount);
}

size_t			EventManager::getSubscribeCount() const
{
	return (m_subscribeCount);
}

//private

EventManager::EventManager(const EventManager& copy) :
	m_epollfd		(copy.m_epollfd),
	m_globals		(copy.m_globals)
{}

EventManager& EventManager::operator=(const EventManager& assign)
{
	(void)assign;
	return (*this);
}