/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.tpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 10:28:44 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 14:59:36 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERSOCKET_TPP

# define SERVERSOCKET_TPP

// C headers
# include <sys/socket.h>


// C++ headers
# include <stdexcept>

// Concrete Project headers

// Interface Project headers
# include "../Abstract/ServerSocket/IServerSocket.hpp"
# include "CommunicationSocket.hpp"

template <
    typename SockAddr
>
class CommunicationSocket;

template <
    typename SockAddr
>
class ServerSocket : public IServerSocket
{
    public:
        
        ServerSocket(const SockAddr& addr, int type, int protocol, IFileDescriptorManager* fdManager = NULL);
        ~ServerSocket();
        

        // implementation of IOnEvents
        void            onClose();
        void            onRead();
        void            onWrite();
        void            onError(); 

        void            setFdManager(IFileDescriptorManager* manager);
        
        void                                                bind();
        void                                                listen();
        UniquePtr<ICommunicationSocket>                     accept();


        //ISocketAddress methods
        struct sockaddr*                    getSockAddr() { return (this->_addr.getSockAddr()); }
        socklen_t*                          getAddrLen() { return (this->_addr.getAddrLen()); };
        int                                 getAddrFamily() const { return (this->_addr.getAddrFamily()); }
        UniquePtr<ISocketAddress>           clone() const { return (this->_addr.clone()); }  


    private:
        SockAddr                            _addr;
        IFileDescriptorManager*             _fdManager;
        
        //copy
        ServerSocket();
        ServerSocket(const ServerSocket& copy);
        ServerSocket& operator=(const ServerSocket& assign);

};

template <typename SockAddr>
ServerSocket<SockAddr>::ServerSocket(const SockAddr& addr, int type, int protocol, IFileDescriptorManager* fdManager) :
    _addr(addr),
    _fdManager(fdManager)
{
    int opt = 1;
    _fd = ::socket(addr.getAddrFamily(), type, protocol);   
    if (_fd == -1)
        throw ParameterException("SocketServer constructor failed", "socket", std::strerror(errno));
    if (::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
        throw ParameterException("SocketServer constructor failed", "setsockopt", std::strerror(errno));
}

template <typename SockAddr>
ServerSocket<SockAddr>::~ServerSocket()
{
    if (_fd != -1)
        ::close(_fd);
}
        
        // implementation of FileDescriptor Functions
template <typename SockAddr>
void            ServerSocket<SockAddr>::onClose() {}

template <typename SockAddr>
void            ServerSocket<SockAddr>::onRead()
{
    assert(_fdManager != NULL);
    
    UniquePtr<ICommunicationSocket> newComm = this->accept();
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
UniquePtr<ICommunicationSocket>      ServerSocket<SockAddr>::accept()
{
    SockAddr newAddress;
    
    int newFd = ::accept(_fd, newAddress.getSockAddr(), newAddress.getAddrLen());
    if (newFd == -1)
        return (NULL);
    return (UniquePtr<ICommunicationSocket>(new CommunicationSocket<SockAddr> (newFd, newAddress)));
}


template <typename SockAddr>
ServerSocket<SockAddr>::ServerSocket()  {_fd = -1;};

template <typename SockAddr>
ServerSocket<SockAddr>::ServerSocket(const ServerSocket<SockAddr>& copy) : _addr(copy._addr), _fdManager(copy._fdManager) {_fd = copy._fd;}

template <typename SockAddr>
ServerSocket<SockAddr>& ServerSocket<SockAddr>::operator=(const ServerSocket<SockAddr>& assign) {(void)assign; return (*this);}


#endif