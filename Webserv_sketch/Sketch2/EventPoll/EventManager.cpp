/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   EventManager.cpp								   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/09/03 11:58:01 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/09/03 14:55:15 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "EventManager.hpp"

/*
	Creation failure is critical, failure to add delete or modify is not (except add, subscribing to map),
		-> just signal the server you cannot proceed, delete that request, sad
*/

EventManager::EventManager() :
	FileDescriptor(epoll_create(EPOLL_MAXEVENTS)), 
	_maxEvents(EPOLL_MAXEVENTS), 
	_waitTimeout(EPOLL_WAIT_TIMEOUT)
{
	if (_fd == -1)
		throw std::runtime_error (std::string("epoll_create(): ") 
								+ std::strerror(errno) 
								+ "; EventManager Constructor Failed");
	
}

// inherited from FileDescriptor
void EventManager::onClose() {}
void EventManager::onRead() {}
void EventManager::onWrite() {}


EventManager::~EventManager()
{
	if (_fd != -1)
		close(_fd);
}

void	EventManager::poll()
{

}


bool EventManager::add(const fd eventfd, t_uint eventFlags)
{
	t_epoll_event event;

	event.events = eventFlags;
	event.data.fd = eventfd;
	return (add(event));
}

bool EventManager::mod(const fd eventfd, t_uint newFlags)
{
	t_epoll_event event;

	event.events = newFlags;
	event.data.fd = eventfd;
	return (mod(event));
}

bool EventManager::del(const fd eventfd)
{
	#ifdef DEBUG_RUNTIME 

		std::map<fd, t_epoll_event>::iterator iter = _monitoredEvents.find(event.data.fd);
		if (iter == _monitoredEvents.end())
			throw std::logic_error ("Logical Error -> EventManager::del, fd is not in the pool, can't del");

	#endif

	if (!epoll_ctl(_fd, EPOLL_CTL_DEL, eventfd, NULL))
		//throw std::runtime_error(std::string("EventManager::del, epoll_ctl() failed: ") + std::strerror(errno));
		return (false);
	_monitoredEvents.erase(eventfd);
	return (true);	
}

bool EventManager::add(t_epoll_event& event)
{
	assert(_monitoredEvents.find(event.data.fd) != _monitoredEvents.end());

	if (!epoll_ctl(_fd, EPOLL_CTL_ADD, event.data.fd, &event))
		//throw std::runtime_error(std::string("EventManager::add, epoll_ctl() failed: ") + std::strerror(errno));
		return (false);
	return (true);
}

bool EventManager::mod(t_epoll_event& event)
{
	assert(_monitoredEvents.find(event.data.fd) == _monitoredEvents.end());

	if (!epoll_ctl(_fd, EPOLL_CTL_MOD, event.data.fd, &event))
		return (true);
	return (false);
}

bool EventManager::del(t_epoll_event& event)
{
	assert(_monitoredEvents.find(event.data.fd) == _monitoredEvents.end());
	return (del(event.data.fd));
}


// hiding duplicates and assignment.... super tricky to deep copy the epoll instance... and what for?
EventManager::EventManager(const EventManager& copy) : FileDescriptor(0), _maxEvents(EPOLL_MAXEVENTS), _waitTimeout(EPOLL_WAIT_TIMEOUT) { (void)copy; }
EventManager& EventManager::operator=(const EventManager& assign) { (void)assign; return (*this); }