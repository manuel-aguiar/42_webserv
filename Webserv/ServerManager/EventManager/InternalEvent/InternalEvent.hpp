/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalEvent.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:40:22 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/17 17:31:45 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNALEVENT_HPP

# define INTERNALEVENT_HPP

# include "../EventManager/EventManager.hpp"
# include "../../../Event/Event.hpp"

class EventManager::InternalEvent : public Event
{
    public:
        InternalEvent();
        ~InternalEvent();

        bool    isInvalid() const;
        
        void    updateSubscription();
        void    unsubscribe();

        void    setSubscribedFd(t_fd fd);
        void    setSubscribedFlags(int flags);
        t_fd    getSubscribedFd() const;
        int     getSubscribedFlags() const;
        
    private:
        InternalEvent(const InternalEvent& copy);
        InternalEvent& operator=(const InternalEvent& assign);
};

#endif