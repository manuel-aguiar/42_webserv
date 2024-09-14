/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:17:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/14 10:56:42 by mmaria-d         ###   ########.fr       */
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
        ServerSocket();
        ServerSocket(const ISocketAddress& addr, int type, int protocol);
        ~ServerSocket();
        
        //move
        ServerSocket(ServerSocket& copy);
        ServerSocket& operator=(ServerSocket& assign);



        // inherited from AServerSocket
        void                                bind();
        void                                listen();
        UniquePtr<ACommunicationSocket>     accept();
    
    private:
        //copy
        ServerSocket(const ServerSocket& copy);
        ServerSocket& operator=(const ServerSocket& assign);

};


#endif