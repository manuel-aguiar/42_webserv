/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IEvent.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 08:00:29 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/17 10:30:25 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IEVENT_HPP

# define IEVENT_HPP

# include "../FileDescriptor/IPollableFd.hpp"

class IEvent
{
    public:
        virtual ~IEvent() {}

        virtual bool            toClose() const = 0;
        virtual bool            toRead() const = 0;
        virtual bool            toWrite() const = 0;
        virtual bool            toError() const = 0;
        virtual int             getFd() const = 0;
};


#endif