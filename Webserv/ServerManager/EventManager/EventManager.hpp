/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:12:10 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 14:29:08 by mmaria-d         ###   ########.fr       */
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
		EventManager(Globals& globals);
		~EventManager();

		//methods
		int							ProcessEvents(int timeOut);

		int							addEvent(const Event& event);
		int							modEvent(const Event& event);
		int							delEvent(const Event& event);
		
		//getters
		t_fd						getEpollFd() const;
		int						  	getWaitCount() const;
		size_t						getSubscribeCount() const;
		
	private:
		t_epoll_event				m_events[MAX_EPOLL_EVENTS];
		size_t						m_subscribeCount;
		t_fd						m_epollfd;
		int							m_waitCount;
		Globals&					m_globals;

		EventManager(const EventManager& copy);
		EventManager& operator=(const EventManager& assign);
};



#endif
