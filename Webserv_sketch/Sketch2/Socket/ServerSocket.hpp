/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:49:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 11:29:16 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP

# include "Socket.hpp"

class ClientSocket;

class ServerSocket : public Socket
{
    public:
        ServerSocket() : Socket() {}
        ServerSocket(const ServerSocket& other) : Socket(other) {}
        ServerSocket& operator=(const ServerSocket& other)
        {
            if (this != &other)
            {
                Socket::operator=(other);
            }
            return *this;
        }
        ~ServerSocket() {}

        bool                bind();
        bool                listen();
        ClientSocket        accept();

};

#endif