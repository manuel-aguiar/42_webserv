/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISocketAddress.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:32:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 14:18:49 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETADDRESS_HPP

# define SOCKETADDRESS_HPP

# include <sys/socket.h>

/*
    Interface for socket addresses depending on the address family the user wants
*/

class ISocketAddress
{
    public:
        virtual ~ISocketAddress() {};
        
        virtual struct sockaddr*    getSockAddr() = 0;
        virtual socklen_t*          getAddrLen() = 0;
        virtual int                 getAddrFamily() const = 0;
        virtual ISocketAddress*     clone() const = 0;
};



#endif