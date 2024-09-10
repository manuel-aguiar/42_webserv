/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:30:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 11:19:56 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP

# define SOCKET_HPP

# include "sys/socket.h"

# include "../FileDescriptor/FileDescriptor.hpp"
# include "SocketAddress/ISocketAddress.hpp"

class Socket : public FileDescriptor
{
    public:
        Socket(int domain, int type, int protocol);
        Socket(const Socket& other);
        Socket& operator=(const Socket& other);
        virtual ~Socket();
    
    protected:
        ISocketAddress* _addr;

        Socket();
};

#endif