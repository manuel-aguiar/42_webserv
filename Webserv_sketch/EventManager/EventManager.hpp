/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:12:10 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 12:55:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTMANAGER_HPP

# define EVENTMANAGER_HPP

# include "../Webserver_Definitions.h"

class Globals;
class Event;

class EventManager
{
    public:
        EventManager(Globals* globals = NULL);
        ~EventManager();

        int                         addEvent(t_fd fd, Event& monitor);
        int                         modEvent(t_fd fd, Event& monitor);
        int                         delEvent(t_fd fd);
        int                         waitEvents(int timeOut);
        void                        distributeEvents();
        const t_epoll_event&        getEvent(int index);

    //private:
        t_fd                _epollfd;
        t_epoll_event       _events[MAX_EPOLL_EVENTS];    
        int                 _waitCount;
        Globals*            _globals;

    private:
        EventManager(const EventManager& copy);
        EventManager& operator=(const EventManager& assign);
};



#endif