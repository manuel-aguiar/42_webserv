/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IEventPoll.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 11:17:21 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 12:10:18 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IEVENTPOLL_HPP

# define IEVENTPOLL_HPP

# include <sys/epoll.h>
# include <unistd.h>

typedef unsigned int t_uint;
typedef struct epoll_event t_epoll_event;
typedef int fd;

class IEventPoll {
    public:
        virtual ~IEventPoll() {}
        virtual void poll() = 0;
		virtual bool add(const fd eventfd, t_uint eventFlags) = 0;
		virtual bool mod(const fd eventfd, t_uint newFlags) = 0;
		virtual bool del(const fd eventfd) = 0;

		virtual bool add(t_epoll_event& event) = 0;
		virtual bool mod(t_epoll_event& event) = 0;
		virtual bool del(t_epoll_event& event) = 0;
};




#endif