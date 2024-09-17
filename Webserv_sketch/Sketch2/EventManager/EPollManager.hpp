/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EPollManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 09:57:24 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/17 15:01:57 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTPOLL_HPP

# define EVENTPOLL_HPP

//C++ headers
# include <cassert>

// C headers
# include <sys/epoll.h>

// Project headers
# include "../FileDescriptor/FileDescriptor.hpp"
# include "../Event/EPollEvent.hpp"
# include "../Exceptions/ParameterException.hpp"

# include "IEventManager.hpp"

//
# ifndef MAX_EPOLL_EVENTS
#  define MAX_EPOLL_EVENTS 64
# endif

class EPollManager : public IEventManager
{
    public:
        EPollManager();
        ~EPollManager();

        // inherited from IEventManager
        void                addEventFd(IEvent& monitor);
        void                modEventFd(IEvent& monitor);
        void                delEventFd(IEvent& monitor);
        int                 waitEvents(int timeOut);
        const   IEvent&     getEvent(int index);

    private:
        EPollEvent          _events[MAX_EPOLL_EVENTS];    
        int                 _waitCount;


        EPollManager(const EPollManager& copy);
        EPollManager& operator=(const EPollManager& assign);
};



#endif