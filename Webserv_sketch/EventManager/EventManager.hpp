/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:12:10 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/30 15:11:04 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTMANAGER_HPP

# define EVENTMANAGER_HPP

# include "../Webserver_Definitions.h"

class Event;

class EventManager
{
    public:
        EventManager();
        ~EventManager();

        void                        addEvent(Event& monitor);
        void                        modEvent(Event& monitor);
        void                        delEvent(Event& monitor);
        int                         waitEvents(int timeOut);
        void                        distributeEvents();
        const t_epoll_event&        getEvent(int index);

    //private:
        t_fd                _epollfd;
        t_epoll_event       _events[MAX_EPOLL_EVENTS];    
        int                 _waitCount;

        EventManager(const EventManager& copy);
        EventManager& operator=(const EventManager& assign);
};



#endif