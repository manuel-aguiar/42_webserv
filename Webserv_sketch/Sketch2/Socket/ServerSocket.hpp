/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:49:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 16:35:56 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP

# include "Socket.hpp"

class ClientSocket;

class ServerSocket : public Socket
{
    public:
        ServerSocket(const ISocketAddress& addr, int type, int protocol, int maxConnections);

        ServerSocket(const ServerSocket& other);
        ServerSocket& operator=(const ServerSocket& other);
        ~ServerSocket();

        ClientSocket*       accept();

    private:
        void                bindListen(int maxConnections);
        ServerSocket();

};

#endif