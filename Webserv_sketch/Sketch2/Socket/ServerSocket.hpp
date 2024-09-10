/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:49:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 11:55:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP

# include "Socket.hpp"

class ClientSocket;

class ServerSocket : public Socket
{
    public:
        ServerSocket(int domain, int type, int protocol);
        ServerSocket(const ServerSocket& other);
        ServerSocket& operator=(const ServerSocket& other);
        ~ServerSocket() {}

        bool                bind();
        bool                listen();
        ClientSocket        accept();

};

#endif