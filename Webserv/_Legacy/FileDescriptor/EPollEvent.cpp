/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EPollEvent.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 09:00:12 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 11:02:15 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "EPollEvent.hpp"

bool    EPollEvent::toRead(const struct epoll_event& event)
{
    return (event.events & EPollEvent::READ);
}

bool    EPollEvent::toWrite(const struct epoll_event& event)
{
    return (event.events & EPollEvent::WRITE);
}

bool    EPollEvent::toClose(const struct epoll_event& event)
{
    return (event.events & EPollEvent::CLOSE);
}

bool    EPollEvent::toError(const struct epoll_event& event)
{
    return (event.events & EPollEvent::ERROR);
}
