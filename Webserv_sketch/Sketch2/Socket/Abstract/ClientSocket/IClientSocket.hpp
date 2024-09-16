/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IClientSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:21:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 10:21:17 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICLIENTSOCKET_HPP

# define ICLIENTSOCKET_HPP

#include "../../FileDescriptor/FileDescriptor.hpp"

class IClientSocket : virtual public FileDescriptor
{
    public:
        virtual ~IClientSocket() {}

        virtual void connect() = 0;
        virtual void disconnect() = 0;
};


#endif