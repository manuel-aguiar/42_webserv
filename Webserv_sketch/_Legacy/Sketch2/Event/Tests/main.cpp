/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 09:14:51 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/17 10:12:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../EPollEvent.hpp"


const EPollEvent* returnEvent()
{
    struct epoll_event event;

    event.data.ptr = &event;
    event.events = 123;
    return (static_cast<EPollEvent*>(&event));
}

int main1(void)
{
    EPollEvent          event(NULL, 0);
    struct epoll_event  event2;

    event2.data.ptr = &event2;
    event2.events = 456;

    EPollEvent          event3(event2);

    return (0);
}