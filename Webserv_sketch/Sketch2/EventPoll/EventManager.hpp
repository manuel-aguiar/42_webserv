/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:11:09 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 14:14:08 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTMANAGER_HPP

# define EVENTMANAGER_HPP

# include "IEventPoll.hpp"

// C Headers
# include <sys/epoll.h>
# include <unistd.h>

// C++ Headers
# include <map>
# include <exception>
# include <stdexcept>
# include <cstring>


typedef unsigned int t_uint;
typedef struct epoll_event t_epoll_event;
typedef int fd;

# define EPOLL_MAXEVENTS 10
# define EPOLL_WAIT_TIMEOUT 10

class EventManager : public IEventPoll
{
	public:
		
		EventManager();
		~EventManager();

		bool add(const fd eventfd, t_uint eventFlags);
		bool mod(const fd eventfd, t_uint newFlags);
		bool del(const fd eventfd);

		bool add(t_epoll_event& event);
		bool mod(t_epoll_event& event);
		bool del(t_epoll_event& event);

		void poll();

	private:
		const fd						_epollfd;
		const t_uint					_maxEvents;
		const t_uint					_waitTimeout;
		t_uint							_triggeredCount;
		t_epoll_event		   			_triggeredEvents[EPOLL_MAXEVENTS];					 
		std::map<fd, t_epoll_event>		_monitoredEvents;
		

		EventManager(const EventManager& copy);
		EventManager& operator=(const EventManager& assign);
};


#endif