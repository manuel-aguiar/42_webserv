/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EPollManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 09:57:24 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:33:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTPOLL_HPP

# define EVENTPOLL_HPP

//C++ headers
# include <cassert>

// C headers
# include <sys/epoll.h>

// Project headers
# include "FileDescriptor.hpp"
# include "EPollEvent.hpp"
# include "ParameterException.hpp"

//
# ifndef MAX_EPOLL_EVENTS
#  define MAX_EPOLL_EVENTS 64
# endif

class EPollManager
{
    public:
        EPollManager();
        ~EPollManager();

        const int                       getFd() const;


        void                            addEventFd(FileDescriptor* fd, const int eventsToMonitor);
        void                            modEventFd(FileDescriptor* fd, const int eventsToMonitor);
        void                            delEventFd(FileDescriptor* fd);
        int                             waitEvents(int timeOut);
        const   struct epoll_event&     getEvent(int index);

    private:
        int                 m_fd;
        struct epoll_event  m_events[MAX_EPOLL_EVENTS];    
        int                 m_waitCount;


        EPollManager(const EPollManager& copy);
        EPollManager& operator=(const EPollManager& assign);
};



#endif