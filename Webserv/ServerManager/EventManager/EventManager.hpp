/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:12:10 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/06 17:46:19 by mmaria-d         ###   ########.fr       */
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

		//methods
		int							addEvent(const Event& event);
		int							modEvent(const Event& event);
		int							delEvent(const Event& event);
		
		int							waitEvents(int timeOut);
		void						distributeEvents();
		const t_epoll_event&		retrieveEvent(int index);

		//getters
		t_fd						getEpollFd() const;
		int						  	getWaitCount() const;

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
