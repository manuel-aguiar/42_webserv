/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocalServerSocket.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:49:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 18:03:01 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP

# include "Socket.hpp"

class RemoteClient;

class LocalServerSocket : public Socket
{
    public:
        LocalServerSocket(const ISocketAddress& addr, int type, int protocol, int maxConnections);

        LocalServerSocket(const LocalServerSocket& other);
        LocalServerSocket& operator=(const LocalServerSocket& other);
        ~LocalServerSocket();

        RemoteClient*       acceptConnection();

    private:
        void                bindListen(int maxConnections);
        LocalServerSocket();

};

#endif