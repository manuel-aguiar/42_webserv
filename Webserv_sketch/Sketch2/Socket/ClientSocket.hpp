/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:44:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 17:16:30 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP

# define CLIENTSOCKET_HPP

# include "AClientSocket.hpp"

template <
    typename SockAddr
>
class ClientSocket : public AClientSocket<SockAddr>
{
    public:
        ClientSocket() {};
        
        ClientSocket(const SockAddr& addr, int type, int protocol) :
            ASocket<SockAddr>(socket(addr.getAddrFamily(), type, protocol), addr) {}

        ~ClientSocket()
        {
            if (this->_fd != -1)
                close(this->_fd);
        }
        
        // implementation of FileDescriptor Functions
        void            onClose() {};
        void            onRead() {};
        void            onWrite() {};  

        //implementation of IClientFunctions
        void connect()
        {
            if (::connect(this->_fd, this->_addr.getSockAddr(), *(this->_addr.getAddrLen())) == -1)
                throw ParameterException("ClientSocket::connect", "connect", std::strerror(errno));
        }
        void disconnect() {};

        //implementation of ICommunicationFunctions
        void send() {};
        void receive()
        {
            char buffer[1024];

            int bytesReceived = read(this->_fd, buffer, sizeof(buffer) - 1);
            if (bytesReceived == -1)
                throw ParameterException("ClientSocket::receive", "read", std::strerror(errno));
            buffer[bytesReceived] = '\0';
            std::cout << "Received: " << std::string(buffer, 0, bytesReceived) << std::endl;
        }

    private:

        ClientSocket(const ClientSocket& copy) : ASocket<SockAddr>(copy) {}
        ClientSocket& operator=(const ClientSocket& assign)
        {
            ASocket<SockAddr>::operator=(assign);
            return (*this);
        }
};


#endif