/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventPoll.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:09:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 12:11:17 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EventPoll_HPP

# define EventPoll_HPP

# include "IEventPoll.hpp"

class EventPoll : public IEventPoll
{
    public:
        EventPoll();
        ~EventPoll();

        EventPoll(const EventPoll& copy);
        EventPoll& operator=(const EventPoll& assign);

        void poll();
};



#endif