/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:17:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 07:45:54 by mmaria-d         ###   ########.fr       */
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
# include "../FileDescriptorManager/FileDescriptorManager.hpp"

// Interface Project headers
# include "AServerSocket.hpp"

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
        
        ServerSocket(IFileDescriptorManager& fdManager, const SockAddr& addr, int type, int protocol) :
            ASocket<SockAddr>(socket(addr.getAddrFamily(), type, protocol), addr),
            _fdManager(fdManager)
        {
            int opt = 1;
            if (this->_fd == -1)
                throw ParameterException("SocketServer constructor failed", "socket", std::strerror(errno));
            if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
                throw ParameterException("SocketServer constructor failed", "setsockopt", std::strerror(errno));
        }

        ~ServerSocket()
        {
            if (this->_fd != -1)
                ::close(this->_fd);
        }
        
        // implementation of FileDescriptor Functions
        void            onClose() {};

        void            onRead()
        {
            UniquePtr<ACommunicationSocket<SockAddr> > newComm = this->accept();
            if (newComm.get() != NULL)
                _fdManager.addFileDescriptor(dynamic_cast<FileDescriptor*>(newComm.release()), true);
        }
        void            onWrite() {};

        void            onError() {}; 

        // implementation of AServerSocket
        void                                            bind()
        {
            if (::bind(this->_fd, this->_addr.getSockAddr(), *(this->_addr.getAddrLen())) == -1)
                throw ParameterException("ServerSocket::bind", "bind", std::strerror(errno));
        }
        
        void                                            listen()
        {
            if (::listen(this->_fd, 10) == -1)
                throw ParameterException("ServerSocket::listen", "listen", std::strerror(errno));
        }

        UniquePtr<ACommunicationSocket<SockAddr> >       accept()
        {
            SockAddr newAddress;
            
            int newFd = ::accept(this->_fd, newAddress.getSockAddr(), newAddress.getAddrLen());
            if (newFd == -1)
                return (NULL);
            return (UniquePtr<ACommunicationSocket<SockAddr> >(new CommunicationSocket<SockAddr> (newFd, newAddress)));
        }
    
    private:
        IFileDescriptorManager&             _fdManager;
        
        //copy
        ServerSocket();
        ServerSocket(const ServerSocket& copy) : ASocket<SockAddr>(copy), _fdManager(copy._fdManager) {}
        ServerSocket& operator=(const ServerSocket& assign) {(void)assign; return (*this);}

};


#endif