/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EPollEvent.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 08:56:24 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 10:53:58 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLLEVENT_HPP

# define EPOLLEVENT_HPP

# include "FileDescriptor.hpp"
# include <sys/epoll.h>

class EPollEvent
{
    public:
        typedef enum    e_events
        {
            READ = EPOLLIN,
            WRITE = EPOLLOUT,
            CLOSE = EPOLLHUP,
            ERROR = EPOLLERR
        }               e_events;

        static bool    toRead(const struct epoll_event& event);
        static bool    toWrite(const struct epoll_event& event);
        static bool    toClose(const struct epoll_event& event);
        static bool    toError(const struct epoll_event& event);

};  


#endif

/*
clear && c++ -Wall -Wextra -Werror -std=c++98 EPollManager.cpp Tests/main.cpp ../Event/EPollEvent.cpp ../Exceptions/ParameterException.cpp ../FileDescriptor/FileDescriptor.cpp -o main
*/