/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:44:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 09:49:31 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP

# define CLIENTSOCKET_HPP

# include "AClientSocket.hpp"

class ClientSocket : public AClientSocket
{
    public:
        ClientSocket();
        ClientSocket(const ISocketAddress& addr, int type, int protocol);
        ~ClientSocket();

        // implementation of FileDescriptor Functions
        void            onClose();
        void            onRead();
        void            onWrite();  

        //implementation of IClientFunctions
        void connect();
        void disconnect();

        //implementation of ICommunicationFunctions
        void send();
        void receive();

    private:
        ClientSocket(const ClientSocket& copy);
        ClientSocket& operator=(const ClientSocket& assign);
};


#endif