/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:17:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 13:48:18 by mmaria-d         ###   ########.fr       */
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
# include "../Exceptions/ParameterException.hpp"

// Interface Project headers
# include "AServerSocket.hpp"

class CommunicationSocket;

//final class
class ServerSocket : public AServerSocket
{
    public:
        
        ServerSocket(const ISocketAddress& addr, int type, int protocol);
        ~ServerSocket();
        
        // inherited from ISocket
        void                            close();

        // inherited from AServerSocket
        void                            bind();
        void                            listen();
        ACommunicationSocket*           accept();

        bool                            operator<(const ServerSocket& other) const;

    private:


        //private
        ServerSocket();

        //avoid duplicate sockets, duplicate fds
        ServerSocket(const ServerSocket& copy);
        ServerSocket& operator=(const ServerSocket& assign);
};


#endif