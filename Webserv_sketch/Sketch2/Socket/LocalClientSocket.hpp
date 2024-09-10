/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocalClientSocket.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:49:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 18:29:49 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP

# include "Socket.hpp"

class RemoteServer;

class LocalClientSocket
{
    public:

        LocalClientSocket(int domain, int type, int protocol);
        LocalClientSocket(const ISocketAddress& addr, int type, int protocol);

        LocalClientSocket(const LocalClientSocket& other);
        LocalClientSocket& operator=(const LocalClientSocket& other);
        ~LocalClientSocket();

        void       requestConnection(const RemoteServer& server);

    private:
        LocalClientSocket();

};

#endif