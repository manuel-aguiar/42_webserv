/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICommunicationHandle.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:21 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 09:38:42 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICOMMUNICATIONFUNCTIONS_HPP

# define ICOMMUNICATIONFUNCTIONS_HPP

#include "../../FileDescriptor/FileDescriptor.hpp"

class ICommunicationHandle : virtual public FileDescriptor
{
    public:
        virtual ~ICommunicationHandle() {}
        virtual void send() = 0;
        virtual void receive() = 0;
};


#endif