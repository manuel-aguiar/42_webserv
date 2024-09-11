/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:44:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 14:42:22 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP

# define CLIENTSOCKET_HPP

# include "AClientSocket.hpp"

class ClientSocket : public AClientSocket
{
    public:
        ClientSocket(const ISocketAddress& addr, int type, int protocol);
        ~ClientSocket();

        //implementation of ASocket
        void close();

        //implementation of IClientFunctions
        void connect();
        void disconnect();

        //implementation of ICommunicationFunctions
        void send();
        void receive();
};


#endif