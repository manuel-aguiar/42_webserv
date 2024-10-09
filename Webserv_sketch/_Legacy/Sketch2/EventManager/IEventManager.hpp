/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IEventManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 08:19:48 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/17 11:38:08 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IEVENTMANAGER_HPP

# define IEVENTMANAGER_HPP

# include "../FileDescriptor/FileDescriptor.hpp"
# include "../Event/IEvent.hpp"
# include "IEventManagerMethods.hpp"

class IEventManager : public FileDescriptor, public IEventManagerMethods
{
    public:
        virtual ~IEventManager() {}

        typedef enum {
            READ = 1,
            WRITE = 2,
            ERROR = 4,
            CLOSE = 8
        } Event;
};


#endif