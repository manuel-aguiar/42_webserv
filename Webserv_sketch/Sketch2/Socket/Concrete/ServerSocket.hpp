/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:17:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 11:40:38 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERSOCKET_HPP

# define SERVERSOCKET_HPP

// C headers
# include <sys/socket.h>


// C++ headers
# include <stdexcept>

// Concrete Project headers
# include "../../FileDescriptor/FileDescriptor.hpp"
# include "../../Exceptions/ParameterException.hpp"
# include "../../FileDescriptorManager/FileDescriptorManager.hpp"

// Interface Project headers
# include "../Abstract/ServerSocket/AServerSocket.hpp"

template <
    typename SockAddr
>
class CommunicationSocket;

//final class
template <
    typename SockAddr
>
class ServerSocket : public AServerSocket<SockAddr>
{
    public:
        
        ServerSocket(const SockAddr& addr, int type, int protocol, IFileDescriptorManager* fdManager = NULL);
        ~ServerSocket();
        
        // implementation of File Descriptor Functions
        void            onClose();
        void            onRead();
        void            onWrite();
        void            onError(); 

        // implementation of IServerSocket Functions
        void            setFdManager(IFileDescriptorManager* manager);
        void            bind();
        void            listen();

        // implementation of IServerSocketAccept Functions
        UniquePtr<ACommunicationSocket<SockAddr> >          accept();

    
    private:
        IFileDescriptorManager*             _fdManager;
        
        //copy
        ServerSocket();
        ServerSocket(const ServerSocket& copy);
        ServerSocket& operator=(const ServerSocket& assign);

};

# include "ServerSocket.tpp"


#endif