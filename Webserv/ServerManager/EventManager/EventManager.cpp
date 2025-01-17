/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:12:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/17 09:53:10 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "EventManager.hpp"
# include "../../Event/Event.hpp"
# include "../../Globals/Globals.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

#include <iostream>

EventManager::EventManager(Globals& globals) :
	m_subscribeCount(0),
	m_epollfd		(-1),
	m_globals		(globals),
	m_maxStaleFd	(0)
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

	std::memset(m_staleEvents, 0, sizeof(m_staleEvents));
}

void EventManager::mf_markFdStale(t_fd fd)
{
	m_maxStaleFd = (fd > m_maxStaleFd) ? fd : m_maxStaleFd;
	size_t index = fd / 8;
	size_t bit = fd % 8;
    m_staleEvents[index] |= (1 << bit);
}

int	EventManager::mf_isFdStale(t_fd fd)
{
	if (fd == -1)
		return (1);
	size_t index = fd / 8;
	size_t bit = fd % 8;
	return ((m_staleEvents[index] & (1 << bit)) != 0);
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
	
	epollEvent.events = event.getMonitoredFlags();
	epollEvent.data.ptr = (void *)&event;

	assert(event.getFd() != -1);

	if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, event.getFd(), &epollEvent) == -1)
	{
		//std::cout << " failed" << std::endl;
		m_globals.logError("EventManager::addEvent, epoll_ctl(): " + std::string(strerror(errno)));
		return (0);
	}
	++m_subscribeCount;

	//std::cout << " passed " << ", subscribe count is " << m_subscribeCount << std::endl;

	return (1);
}

int                EventManager::modEvent(const Event& event)
{
	t_epoll_event epollEvent = (t_epoll_event){};

	epollEvent.events = event.getMonitoredFlags();
	epollEvent.data.ptr = (void *)&event;

	assert(event.getFd() != -1);

	if (epoll_ctl(m_epollfd, EPOLL_CTL_MOD, event.getFd(), &epollEvent) == -1)
	{
		m_globals.logError("EventManager::delEvent, epoll_ctl(): " + std::string(strerror(errno)));
		return (0);
	}
	return (1);
}

int                 EventManager::delEvent(const Event& event, bool markAsStale)
{
	t_fd fd;
	//std::cout << "EventManager::delEvent fd: " << event.getFd();
	fd = event.getFd();

	assert(fd != -1);

	if (epoll_ctl(m_epollfd, EPOLL_CTL_DEL, fd, NULL) == -1)
	{
		m_globals.logError("EventManager::delEvent, epoll_ctl(): " + std::string(strerror(errno)));

		//std::cout << " FAILED: " << std::string(strerror(errno)) << std::endl;

		return (0);
	}
	if (markAsStale)
		mf_markFdStale(fd);

	--m_subscribeCount;

	//std::cout << " passed, subscribe count is " << m_subscribeCount << std::endl;

	return (1);
}

int                 EventManager::ProcessEvents(int timeOut)
{
	Event*	event;
	t_fd	fd;
	int		waitCount;

	//std::cout << "\n\n\t\t\t\t\t EVENT LOOP TURN HAS STARTED \n\n\n" << std::endl;

	waitCount = epoll_wait(m_epollfd, m_events, MAX_EPOLL_EVENTS, timeOut);

	if (waitCount == -1)
	{
		m_globals.logError("EventManager::ProcessEvents, epoll_wait(): " + std::string(strerror(errno)));
		return (waitCount);
	}
	for (int i = 0; i < waitCount; i++)
	{
		event = static_cast<Event*>(m_events[i].data.ptr);
		fd = event->getFd();
		if (mf_isFdStale(fd))
			continue ;
		event->setTriggeredFlags(m_events[i].events);
		event->handle();

		//std::cout << "\n\n\t\t\t\t\t EVENT LOOP TURN HAS PASSED \n\n\n" << std::endl;
	}

	std::memset(m_staleEvents, 0, sizeof(size_t) * ((m_maxStaleFd / (sizeof(size_t) * 8)) + 1));
	m_maxStaleFd = 0;
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