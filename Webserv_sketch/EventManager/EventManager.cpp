/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:58:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/03 13:59:07 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventManager.hpp"

EventManager::EventManager(const t_uint maxEvents) :
	_epollfd(epoll_create(EPOLL_MAXEVENTS)), 
	_maxEvents(EPOLL_MAXEVENTS)
{
	if (_epollfd == -1)
		throw std::runtime_error (std::string("epoll_create(): ") 
								+ std::strerror(errno) 
								+ "; EventManager Constructor Failed");
	
}


bool EventManager::subscribe(const fd eventfd, t_uint eventFlags)
{
	t_epoll_event event;

	event.events = eventFlags;
	event.data.fd = eventfd;
	subscribe(event);
}

bool EventManager::modify(const fd eventfd, t_uint newFlags)
{
	t_epoll_event event;

	event.events = newFlags;
	event.data.fd = eventfd;
	modify(event);
}

bool EventManager::unsubscribe(const fd eventfd)
{
	#ifdef DEBUG_RUNTIME 

		std::map<fd, t_epoll_event>::iterator iter = _monitoredEvents.find(event.data.fd);
		if (iter == _monitoredEvents.end())
			throw std::logic_error ("Logical Error -> EventManager::unsubscribe, fd is not in the pool, can't unsubscribe");

	#endif

	if (!epoll_ctl(_epollfd, EPOLL_CTL_DEL, eventfd, NULL))
		throw std::runtime_error(std::string("EventManager::unsubscribe, epoll_ctl() failed: ") + std::strerror(errno));
	_monitoredEvents.erase(eventfd);
	return (1);	
}

bool EventManager::subscribe(t_epoll_event& event)
{
	#ifdef DEBUG_RUNTIME 

		std::map<fd, t_epoll_event>::iterator iter = _monitoredEvents.find(event.data.fd);
		if (iter != _monitoredEvents.end())
			throw std::logic_error ("Logical Error -> EventManager::subscribe, fd is already subscribed to epoll");

	#endif

	if (!epoll_ctl(_epollfd, EPOLL_CTL_ADD, event.data.fd, &event))
		throw std::runtime_error(std::string("EventManager::subscribe, epoll_ctl() failed: ") + std::strerror(errno));
	return (1);
}

bool EventManager::modify(t_epoll_event& event)
{
	#ifdef DEBUG_RUNTIME 

		std::map<fd, t_epoll_event>::iterator iter = _monitoredEvents.find(event.data.fd);
		if (iter == _monitoredEvents.end())
			throw std::logic_error ("Logical Error -> EventManager::modify, fd is not in the pool, can't unsubscribe");

	#endif

	if (!epoll_ctl(_epollfd, EPOLL_CTL_MOD, event.data.fd, &event))
		throw std::runtime_error(std::string("EventManager::modify, epoll_ctl() failed: ") + std::strerror(errno));

	return (1);
}

bool EventManager::unsubscribe(t_epoll_event& event)
{

	#ifdef DEBUG_RUNTIME 

		std::map<fd, t_epoll_event>::iterator iter = _monitoredEvents.find(event.data.fd);
		if (iter == _monitoredEvents.end())
			throw std::logic_error ("Logical Error -> EventManager::unsubscribe, fd is not in the pool, can't modify");

	#endif

	return (unsubscribe(event.data.fd));
}


// hiding duplicates and assignment.... super tricky to deep copy the epoll instance... and what for?
EventManager::EventManager() : _epollfd(0), _maxEvents(EPOLL_MAXEVENTS) {}
EventManager::EventManager(const EventManager& copy) : _epollfd(0), _maxEvents(EPOLL_MAXEVENTS) { (void)copy; }
EventManager& EventManager::operator=(const EventManager& assign) { return (*this); }