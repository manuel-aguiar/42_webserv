/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:44:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 17:49:41 by mmaria-d         ###   ########.fr       */
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

        //move
        ClientSocket(ClientSocket& moveCopy);
        ClientSocket& operator=(ClientSocket& moveAssign);

        //copy
        ClientSocket(const ClientSocket& copy);
        ClientSocket& operator=(const ClientSocket& assign);

        //implementation of IClientFunctions
        void connect();
        void disconnect();

        //implementation of ICommunicationFunctions
        void send();
        void receive();
};


#endif