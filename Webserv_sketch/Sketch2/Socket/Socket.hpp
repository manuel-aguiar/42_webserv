/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:30:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 11:54:57 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP

# define SOCKET_HPP

// C++ headers
# include <string>
# include <cstring>
# include <stdexcept>

//C Headers
# include "sys/socket.h"


//Project headers
# include "../FileDescriptor/FileDescriptor.hpp"
# include "SocketAddress/ISocketAddress.hpp"
# include "SocketAddress/SocketAddressFactory.hpp"

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