/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:50:25 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 09:54:18 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP

# define CLIENTSOCKET_HPP

/*
    ServerSocket and ClientSocket must be aware of each other....
    ClientSocket can only be created by the server
    ServerSocket creates ClientSockets
*/

#include "Socket.hpp"

class ServerSocket;

class ClientSocket : public Socket
{
    public:
        ClientSocket(const ServerSocket& server);
        ClientSocket(const ClientSocket& other);
        ClientSocket& operator=(const ClientSocket& other);
        ~ClientSocket();

    
    private:
        ClientSocket();
};



#endif
