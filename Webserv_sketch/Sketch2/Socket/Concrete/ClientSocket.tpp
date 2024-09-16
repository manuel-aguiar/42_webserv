/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.tpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:44:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 15:35:03 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_TPP

# define CLIENTSOCKET_TPP

# include "../Abstract/ClientSocket/IClientSocket.hpp"
# include "../../FileDescriptor/IOnEvents.hpp"
template <
    typename SockAddr
>
class ClientSocket : 
    public IClientSocket,
    public IOnEvents
{
    public:
        ClientSocket()
            {_fd = -1;}
        
        ClientSocket(const SockAddr& addr, int type, int protocol) :
            _addr(addr)
        {
            _fd = ::socket(addr.getAddrFamily(), type, protocol);
            if (_fd == -1)
                throw ParameterException("SocketServer constructor failed", "socket", std::strerror(errno));
        }

        ~ClientSocket()
        {
            if (_fd != -1)
                ::close(_fd);
        }
        
        // implementation of IOnEvents
        void            onClose() {};
        void            onRead() {};
        void            onWrite() {};  
        void            onError() {}; 
        
        //implementation of IClientMethods
        void connect()
        {
            if (::connect(_fd, _addr.getSockAddr(), *(_addr.getAddrLen())) == -1)
                throw ParameterException("ClientSocket::connect", "connect", std::strerror(errno));
        }
        void disconnect() {};

        //implementation of ICommunicationMethods
        void send() {};
        void receive()
        {
            char buffer[1024];

            int bytesReceived = read(_fd, buffer, sizeof(buffer) - 1);
            if (bytesReceived == -1)
                throw ParameterException("ClientSocket::receive", "read", std::strerror(errno));
            buffer[bytesReceived] = '\0';
            std::cout << "Received: " << std::string(buffer, 0, bytesReceived) << std::endl;
        }

        // implementation of ISocketAddress methods
        struct sockaddr*                    getSockAddr() { return (_addr.getSockAddr()); }
        socklen_t*                          getAddrLen() { return (_addr.getAddrLen()); };
        int                                 getAddrFamily() const { return (_addr.getAddrFamily()); }
        UniquePtr<ISocketAddress>           clone() const { return (_addr.clone()); }      

    private:
        SockAddr             _addr;


        ClientSocket(const ClientSocket& copy) : _addr(copy._addr) {_fd = copy._fd;}
        ClientSocket& operator=(const ClientSocket& assign)
        {
            if(this == &assign)
                return (*this);
            _fd = assign._fd;
            _addr(assign._addr);
            return (*this);
        }
};


#endif