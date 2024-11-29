/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:12:10 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/29 10:01:17 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTMANAGER_HPP

# define EVENTMANAGER_HPP

# include "../../GenericUtils/Webserver_Definitions.h"

class Globals;
class Event;

class EventManager
{
	public:
		EventManager(Globals* globals = NULL);
		~EventManager();

		int							addEvent(t_fd fd, Event& monitor);
		int							modEvent(t_fd fd, Event& monitor);
		int							delEvent(t_fd fd);
		int							waitEvents(int timeOut);
		void						distributeEvents();
		const t_epoll_event&		retrieveEvent(int index);

		const t_fd&					getEpollFd() const;
		const int&				  	getWaitCount() const;

	private:
		t_fd						m_epollfd;
		t_epoll_event				m_events[MAX_EPOLL_EVENTS];
		int							m_waitCount;
		Globals*					m_globals;

	private:
		EventManager(const EventManager& copy);
		EventManager& operator=(const EventManager& assign);
};



#endif
