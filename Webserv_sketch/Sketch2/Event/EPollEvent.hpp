/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EPollEvent.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 08:56:24 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/17 10:47:19 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLLEVENT_HPP

# define EPOLLEVENT_HPP

# include "IEvent.hpp"

# include <sys/epoll.h>

class EPollEvent : public epoll_event, public IEvent
{
    public:
        EPollEvent();
        EPollEvent(IPollableFd* fdToMonitor, const int eventsToMonitor);
        
        ~EPollEvent();
        EPollEvent(const EPollEvent& copy);
        EPollEvent& operator=(const EPollEvent& assign);

        EPollEvent(const struct epoll_event& copy);
        EPollEvent& operator=(const struct epoll_event& assign);

        bool            toClose() const;
        bool            toRead() const;
        bool            toWrite() const;
        bool            toError() const;

        bool operator==(const struct epoll_event& other) const;

        int             getFd() const;

};  


#endif

/*
clear && c++ -Wall -Wextra -Werror -std=c++98 EPollManager.cpp Tests/main.cpp ../Event/EPollEvent.cpp ../Exceptions/ParameterException.cpp ../FileDescriptor/FileDescriptor.cpp -o main
*/