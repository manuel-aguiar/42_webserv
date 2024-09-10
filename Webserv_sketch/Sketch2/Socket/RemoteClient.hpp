/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RemoteClient.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:50:25 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 18:09:13 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REMOTECLIENT_HPP

# define REMOTECLIENT_HPP

/*
    ServerSocket and ClientSocket must be aware of each other....
    ClientSocket can only be created by the server
    ServerSocket creates ClientSockets
*/

#include "Socket.hpp"

class RemoteClient
{
    public:
        RemoteClient(int acceptfd, const Socket& socket);
        RemoteClient(const RemoteClient& copy);
        RemoteClient& operator=(const RemoteClient& assign);
        ~RemoteClient();
        
    private:
        Socket   _socket;
        RemoteClient();
};



#endif
