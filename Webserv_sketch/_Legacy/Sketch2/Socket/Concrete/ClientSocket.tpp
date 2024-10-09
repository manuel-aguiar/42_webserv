/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.tpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:44:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:12:06 by mmaria-d         ###   ########.fr       */
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
            {m_fd = -1;}
        
        ClientSocket(const SockAddr& addr, int type, int protocol) :
            m_addr(addr)
        {
            m_fd = ::socket(addr.getAddrFamily(), type, protocol);
            if (m_fd == -1)
                throw ParameterException("SocketServer constructor failed", "socket", std::strerror(errno));
        }

        ~ClientSocket()
        {
            if (m_fd != -1)
                ::close(m_fd);
        }
        
        // implementation of IOnEvents
        void            onClose() {};
        void            onRead() {};
        void            onWrite() {};  
        void            onError() {}; 
        
        //implementation of IClientMethods
        void connect()
        {
            if (::connect(m_fd, m_addr.getSockAddr(), *(m_addr.getAddrLen())) == -1)
                throw ParameterException("ClientSocket::connect", "connect", std::strerror(errno));
        }
        void disconnect() {};

        //implementation of ICommunicationMethods
        void send() {};
        void receive()
        {
            char buffer[1024];

            int bytesReceived = read(m_fd, buffer, sizeof(buffer) - 1);
            if (bytesReceived == -1)
                throw ParameterException("ClientSocket::receive", "read", std::strerror(errno));
            buffer[bytesReceived] = '\0';
            std::cout << "Received: " << std::string(buffer, 0, bytesReceived) << std::endl;
        }

        // implementation of ISocketAddress methods
        struct sockaddr*                    getSockAddr() { return (m_addr.getSockAddr()); }
        socklen_t*                          getAddrLen() { return (m_addr.getAddrLen()); };
        int                                 getAddrFamily() const { return (m_addr.getAddrFamily()); }
        UniquePtr<ISocketAddress>           clone() const { return (m_addr.clone()); }      

    private:
        SockAddr             m_addr;


        ClientSocket(const ClientSocket& copy) : m_addr(copy.m_addr) {m_fd = copy.m_fd;}
        ClientSocket& operator=(const ClientSocket& assign)
        {
            if(this == &assign)
                return (*this);
            m_fd = assign.m_fd;
            m_addr(assign.m_addr);
            return (*this);
        }
};


#endif