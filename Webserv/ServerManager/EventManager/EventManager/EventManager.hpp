/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:12:10 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/17 17:40:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTMANAGER_HPP

# define EVENTMANAGER_HPP

# include "../../../GenericUtils/Webserver_Definitions.h"

class Globals;
class Event;

# define MAX_EPOLL_FDS 1000000

class EventManager
{
	public:
		EventManager(Globals& globals);
		~EventManager();

		//methods
		int							ProcessEvents(int timeOut);

		int							addEvent(Event& event, bool markAsStale = true);
		int							modEvent(Event& event, bool markAsStale = true);
		int							delEvent(Event& event, bool markAsStale = true);
		
		//getters
		size_t						getSubscribeCount() const;
		
	private:
		class						InternalEvent;
		
		size_t						m_subscribeCount;
		t_fd						m_epollfd;
		Globals&					m_globals;
		
		t_fd						m_maxStaleFd;	
		t_epoll_event				m_events[MAX_EPOLL_EVENTS];
		t_byte						m_staleEvents[(MAX_EPOLL_FDS / 8) + 1];

		void						mf_markFdStale(t_fd fd);
		int							mf_isFdStale(t_fd fd);
		
		EventManager(const EventManager& copy);
		EventManager& operator=(const EventManager& assign);
};



#endif
