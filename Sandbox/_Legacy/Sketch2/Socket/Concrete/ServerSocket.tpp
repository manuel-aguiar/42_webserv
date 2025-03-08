/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.tpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 10:28:44 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:12:06 by mmaria-d         ###   ########.fr       */
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
# include "../../FileDescriptor/IOnEvents.hpp"
# include "../Abstract/ServerSocket/IServerSocket.hpp"
# include "CommunicationSocket.hpp"

template <
    typename SockAddr
>
class CommunicationSocket;

template <
    typename SockAddr
>
class ServerSocket : 
    public IServerSocket,
    public IOnEvents
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
        UniquePtr<ICommunicationSocket>                     listenerAccept();


        //ISocketAddress methods
        struct sockaddr*                    getSockAddr() { return (this->m_addr.getSockAddr()); }
        socklen_t*                          getAddrLen() { return (this->m_addr.getAddrLen()); };
        int                                 getAddrFamily() const { return (this->m_addr.getAddrFamily()); }
        UniquePtr<ISocketAddress>           clone() const { return (this->m_addr.clone()); }  


    private:
        SockAddr                            m_addr;
        IFileDescriptorManager*             _fdManager;
        
        //copy
        ServerSocket();
        ServerSocket(const ServerSocket& copy);
        ServerSocket& operator=(const ServerSocket& assign);

};

template <typename SockAddr>
ServerSocket<SockAddr>::ServerSocket(const SockAddr& addr, int type, int protocol, IFileDescriptorManager* fdManager) :
    m_addr(addr),
    _fdManager(fdManager)
{
    int opt = 1;
    m_fd = ::socket(addr.getAddrFamily(), type, protocol);   
    if (m_fd == -1)
        throw ParameterException("SocketServer constructor failed", "socket", std::strerror(errno));
    if (::setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
        throw ParameterException("SocketServer constructor failed", "setsockopt", std::strerror(errno));
}

template <typename SockAddr>
ServerSocket<SockAddr>::~ServerSocket()
{
    if (m_fd != -1)
        ::close(m_fd);
}
        
        // implementation of IPollableOnEvents Functions
template <typename SockAddr>
void            ServerSocket<SockAddr>::onClose() {}

template <typename SockAddr>
void            ServerSocket<SockAddr>::onRead()
{
    assert(_fdManager != NULL);
    
    UniquePtr<ICommunicationSocket> newComm = this->listenerAccept();
    if (newComm.get() != NULL)
        _fdManager->addFd(dynamic_cast<FileDescriptor*>(newComm.release()), true);
    
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
    if (::bind(this->m_fd, this->m_addr.getSockAddr(), *(this->m_addr.getAddrLen())) == -1)
        throw ParameterException("ServerSocket::bind", "bind", std::strerror(errno));
}
template <typename SockAddr>
void            ServerSocket<SockAddr>::listen()
{
    if (::listen(this->m_fd, 10) == -1)
        throw ParameterException("ServerSocket::listen", "listen", std::strerror(errno));
}
template <typename SockAddr>
UniquePtr<ICommunicationSocket>      ServerSocket<SockAddr>::listenerAccept()
{
    SockAddr newAddress;
    
    int newFd = ::listenerAccept(m_fd, newAddress.getSockAddr(), newAddress.getAddrLen());
    if (newFd == -1)
        return (NULL);
    return (UniquePtr<ICommunicationSocket>(new CommunicationSocket<SockAddr> (newFd, newAddress)));
}


template <typename SockAddr>
ServerSocket<SockAddr>::ServerSocket()  {m_fd = -1;};

template <typename SockAddr>
ServerSocket<SockAddr>::ServerSocket(const ServerSocket<SockAddr>& copy) : m_addr(copy.m_addr), _fdManager(copy._fdManager) {m_fd = copy.m_fd;}

template <typename SockAddr>
ServerSocket<SockAddr>& ServerSocket<SockAddr>::operator=(const ServerSocket<SockAddr>& assign) {(void)assign; return (*this);}


#endif