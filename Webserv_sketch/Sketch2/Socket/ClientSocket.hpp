/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:50:25 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 17:14:16 by mmaria-d         ###   ########.fr       */
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

class ClientSocket : public Socket
{
    public:
        ClientSocket(const int listeningFd, const ISocketAddress& addr);
        ClientSocket(const ClientSocket& other);
        ClientSocket& operator=(const ClientSocket& other);
        ~ClientSocket();

    
    private:
        ClientSocket();
};



#endif
