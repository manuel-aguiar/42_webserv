/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.tpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 10:28:44 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 11:57:04 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERSOCKET_TPP

# define SERVERSOCKET_TPP

// C headers
# include <sys/socket.h>


// C++ headers
# include <stdexcept>

// Concrete Project headers
# include "../../FileDescriptor/FileDescriptor.hpp"
# include "../../Exceptions/ParameterException.hpp"
# include "../../FileDescriptorManager/IFileDescriptorManager.hpp"

// Interface Project headers
# include "../Abstract/ServerSocket/AServerSocket.hpp"
# include "CommunicationSocket.hpp"

template <
    typename SockAddr
>
class CommunicationSocket;

template <
    typename SockAddr
>
class ServerSocket : public AServerSocket<SockAddr>
{
    public:
        
        ServerSocket(const SockAddr& addr, int type, int protocol, IFileDescriptorManager* fdManager = NULL);
        ~ServerSocket();
        
        void            onClose();
        void            onRead();
        void            onWrite();
        void            onError(); 

        void            setFdManager(IFileDescriptorManager* manager);
        
        void                                                bind();
        void                                                listen();
        UniquePtr<ACommunicationSocket<SockAddr> >          accept();
    
    private:
        IFileDescriptorManager*             _fdManager;
        
        //copy
        ServerSocket();
        ServerSocket(const ServerSocket& copy);
        ServerSocket& operator=(const ServerSocket& assign);

};

template <typename SockAddr>
ServerSocket<SockAddr>::ServerSocket(const SockAddr& addr, int type, int protocol, IFileDescriptorManager* fdManager) :
    ASocket<SockAddr>(socket(addr.getAddrFamily(), type, protocol), addr),
    _fdManager(fdManager)
{
    int opt = 1;
    if (this->_fd == -1)
        throw ParameterException("SocketServer constructor failed", "socket", std::strerror(errno));
    if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
        throw ParameterException("SocketServer constructor failed", "setsockopt", std::strerror(errno));
}

template <typename SockAddr>
ServerSocket<SockAddr>::~ServerSocket()
{
    if (this->_fd != -1)
        ::close(this->_fd);
}
        
        // implementation of FileDescriptor Functions
template <typename SockAddr>
void            ServerSocket<SockAddr>::onClose() {}

template <typename SockAddr>
void            ServerSocket<SockAddr>::onRead()
{
    assert(_fdManager != NULL);
    
    UniquePtr<ACommunicationSocket<SockAddr> > newComm = this->accept();
    if (newComm.get() != NULL)
        _fdManager->addFileDescriptor(dynamic_cast<FileDescriptor*>(newComm.release()), true);
    
}
template <typename SockAddr>
void            ServerSocket<SockAddr>::onWrite() {};

template <typename SockAddr>
void            ServerSocket<SockAddr>::onError() {}; 

template <typename SockAddr>
void            ServerSocket<SockAddr>::setFdManager(IFileDescriptorManager* manager) {_fdManager = manager;}

        // implementation of AServerSocket
template <typename SockAddr>
void            ServerSocket<SockAddr>::bind()
{
    if (::bind(this->_fd, this->_addr.getSockAddr(), *(this->_addr.getAddrLen())) == -1)
        throw ParameterException("ServerSocket::bind", "bind", std::strerror(errno));
}
template <typename SockAddr>
void            ServerSocket<SockAddr>::listen()
{
    if (::listen(this->_fd, 10) == -1)
        throw ParameterException("ServerSocket::listen", "listen", std::strerror(errno));
}
template <typename SockAddr>
UniquePtr<ACommunicationSocket<SockAddr> >      ServerSocket<SockAddr>::accept()
{
    SockAddr newAddress;
    
    int newFd = ::accept(this->_fd, newAddress.getSockAddr(), newAddress.getAddrLen());
    if (newFd == -1)
        return (NULL);
    return (UniquePtr<ACommunicationSocket<SockAddr> >(new CommunicationSocket<SockAddr> (newFd, newAddress)));
}
    

template <typename SockAddr>
ServerSocket<SockAddr>::ServerSocket() {};

template <typename SockAddr>
ServerSocket<SockAddr>::ServerSocket(const ServerSocket<SockAddr>& copy) : ASocket<SockAddr>(copy), _fdManager(copy._fdManager) {}

template <typename SockAddr>
ServerSocket<SockAddr>& ServerSocket<SockAddr>::operator=(const ServerSocket<SockAddr>& assign) {(void)assign; return (*this);}


#endif