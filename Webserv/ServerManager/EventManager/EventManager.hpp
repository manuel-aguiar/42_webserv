/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:12:10 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/02 11:53:13 by mmaria-d         ###   ########.fr       */
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

		int							addEvent(const t_fd fd, const Event& monitor);
		int							modEvent(const t_fd fd, const Event& monitor);
		int							delEvent(const t_fd fd);
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
