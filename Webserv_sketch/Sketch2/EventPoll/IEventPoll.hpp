/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IEventPoll.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 11:17:21 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 11:57:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IEVENTPOLL_HPP

# define IEVENTPOLL_HPP

class IEventPoll {
    public:
        virtual void poll() = 0;
		virtual bool add(const fd eventfd, t_uint eventFlags);
		virtual bool mod(const fd eventfd, t_uint newFlags);
		virtual bool del(const fd eventfd);

		virtual bool add(t_epoll_event& event);
		virtual bool mod(t_epoll_event& event);
		virtual bool del(t_epoll_event& event);
};




#endif