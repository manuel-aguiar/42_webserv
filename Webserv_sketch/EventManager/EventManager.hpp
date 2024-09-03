/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 09:28:49 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/03 13:58:40 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTMANAGER_HPP

# define EVENTMANAGER_HPP

# include "IEventManager.hpp"

// C Headers
# include <sys/epoll.h>

// C++ Headers
# include <map>
# include <exception>
# include <stdexcept>
# include <cstring>


typedef unsigned int t_uint;
typedef struct epoll_event t_epoll_event;
typedef int fd;

# define EPOLL_MAXEVENTS 10

class EventManager : public IEventManager
{
	public:
		
		EventManager(const t_uint maxEvents);
		~EventManager();

		bool subscribe(const fd eventfd, t_uint eventFlags);
		bool modify(const fd eventfd, t_uint newFlags);
		bool unsubscribe(const fd eventfd);

		bool subscribe(t_epoll_event& event);
		bool modify(t_epoll_event& event);
		bool unsubscribe(t_epoll_event& event);

	private:
		const fd						_epollfd;
		const t_uint            		_maxEvents;
		t_epoll_event           		_triggeredEvents[EPOLL_MAXEVENTS];                     
		std::map<fd, t_epoll_event>		_monitoredEvents;
		

		EventManager();
		EventManager(const EventManager& copy);
		EventManager& operator=(const EventManager& assign);
};


#endif