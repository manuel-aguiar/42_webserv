/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IEventManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 08:19:48 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/17 10:37:49 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IEVENTMANAGER_HPP

# define IEVENTMANAGER_HPP

# include "../FileDescriptor/IPollableFd.hpp"
# include "../Event/IEvent.hpp"

class IEventManager
{
    public:
        virtual ~IEventManager() {}

        virtual void                addEventFd(IEvent& fd);
        virtual void                modEventFd(IEvent& fd);
        virtual void                delEventFd(IEvent& fd);

        virtual int                 waitEvents(int timeOut) = 0;
        virtual const   IEvent&     getEvent(int index) = 0;
};


#endif