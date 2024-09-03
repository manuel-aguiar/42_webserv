/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:58:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/03 13:00:42 by mmaria-d         ###   ########.fr       */
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

EventManager::~EventManager()
{
	
}


int EventManager::subscribe(const fd eventfd, t_uint eventFlags)
{

}

int EventManager::modify(const fd eventfd, t_uint newFlags)
{

}

int EventManager::unsubscribe(const fd eventfd)
{

}






// hiding duplicates and assignment.... super tricky to deep copy the epoll instance... and what for?
EventManager::EventManager() : _epollfd(0), _maxEvents(EPOLL_MAXEVENTS) {}
EventManager::EventManager(const EventManager& copy) : _epollfd(0), _maxEvents(EPOLL_MAXEVENTS) { (void)copy; }
EventManager& EventManager::operator=(const EventManager& assign) { return (*this); }