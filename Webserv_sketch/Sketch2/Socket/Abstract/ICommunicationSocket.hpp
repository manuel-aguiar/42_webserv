/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICommunicationSocket.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:21 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 10:19:36 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICOMMUNICATIONSOCKET_HPP

# define ICOMMUNICATIONSOCKET_HPP

#include "../../FileDescriptor/FileDescriptor.hpp"

class ICommunicationSocket : virtual public FileDescriptor
{
    public:
        virtual ~ICommunicationSocket() {}
        virtual void send() = 0;
        virtual void receive() = 0;
};


#endif