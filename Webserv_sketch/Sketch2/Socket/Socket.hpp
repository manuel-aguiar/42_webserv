/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:30:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 14:21:21 by mmaria-d         ###   ########.fr       */
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
        virtual ~Socket();
    
    protected:
        ISocketAddress* _addr;

        
        Socket(int addressFamily);
        Socket(const Socket& copy);
        Socket& operator=(const Socket& assign);
    
    private:
        Socket();
};

#endif