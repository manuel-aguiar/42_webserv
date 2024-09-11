/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:17:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 09:46:26 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERSOCKET_HPP

# define SERVERSOCKET_HPP

// C headers
# include <sys/socket.h>


// C++ headers
# include <stdexcept>

// Concrete Project headers
# include "../FileDescriptor/FileDescriptor.hpp"

// Interface Project headers
# include "SocketAddress/ISocketAddress.hpp"
# include "IServerSocket.hpp"


//final class
class ServerSocket : public IServerSocket
{
    public:
        
        ServerSocket(const ISocketAddress& addr, int type, int protocol);

        ServerSocket(const ServerSocket& copy);
        ServerSocket& operator=(const ServerSocket& assign);
        ~ServerSocket();

        // inherited from ISocket
        void                    close();

        // inherited from IServerSocket
        void                    bind();
        void                    listen();
        ISendReceiveSocket*     accept();
    
    private:
        FileDescriptor          _fd;
        ISocketAddress*         _addr;



        ServerSocket();
};


#endif