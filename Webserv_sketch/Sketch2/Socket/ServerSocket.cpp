/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:23:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 11:36:28 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerSocket.hpp"
# include "CommunicationSocket.hpp"

ServerSocket::ServerSocket(IFileDescriptorManager& fdManager, const ISocketAddress& addr, int type, int protocol) : 
    ASocket(socket(addr.getAddrFamily(), type, protocol), addr),
    _fdManager(fdManager)
{
    int opt = 1;
    if (_fd == -1)
        throw ParameterException("SocketServer constructor failed", "socket", std::strerror(errno));
    if (_addr == NULL)
        throw ParameterException("SocketServer constructor failed", "new", std::strerror(errno));
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
        throw ParameterException("SocketServer constructor failed", "setsockopt", std::strerror(errno));
}

void    ServerSocket::bind()
{
    if (::bind(_fd, _addr->getSockAddr(), *(_addr->getAddrLen())) == -1)
        throw ParameterException("ServerSocket::bind", "bind", std::strerror(errno));
}

void    ServerSocket::listen()
{
    if (::listen(_fd, 10) == -1)
        throw ParameterException("ServerSocket::listen", "listen", std::strerror(errno));
}

UniquePtr<ACommunicationSocket>    ServerSocket::accept()
{
    UniquePtr<ISocketAddress>               newAddr = UniquePtr<ISocketAddress> (_addr->clone());
    
    int newFd = ::accept(_fd, newAddr->getSockAddr(), newAddr->getAddrLen());
    if (newFd == -1)
        return (NULL);
    return (UniquePtr<ACommunicationSocket>(new CommunicationSocket(newFd, *newAddr)));
}

void    ServerSocket::onClose()     {}
void    ServerSocket::onRead()
{
    UniquePtr<ACommunicationSocket> newComm = accept();
    if (newComm.get() != NULL)
        _fdManager.addFileDescriptor(UniquePtr<FileDescriptor>(dynamic_cast<FileDescriptor*>(newComm.release())));
}
void    ServerSocket::onWrite()     {}



ServerSocket::~ServerSocket()
{
    if (_fd != -1)
        close(_fd);
}


//private, can't copy
ServerSocket::ServerSocket(const ServerSocket& moveCopy) : ASocket(moveCopy), _fdManager(moveCopy._fdManager) {(void)moveCopy;}
ServerSocket& ServerSocket::operator=(const ServerSocket& moveAssign) {(void)moveAssign; return (*this);}
    