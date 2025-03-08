/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IEventManagerMethods.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:57:39 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/17 10:58:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IEVENTMANAGERMETHODS_HPP

# define IEVENTMANAGERMETHODS_HPP

# include "../FileDescriptor/IPollableFd.hpp"
# include "../Event/IEvent.hpp"

class IEventManagerMethods
{
    public:
        virtual ~IEventManagerMethods() {}

        virtual void                addEventFd(IEvent& fd);
        virtual void                modEventFd(IEvent& fd);
        virtual void                delEventFd(IEvent& fd);
        virtual int                 waitEvents(int timeOut) = 0;
        virtual const   IEvent&     getEvent(int index) = 0;
};



#endif