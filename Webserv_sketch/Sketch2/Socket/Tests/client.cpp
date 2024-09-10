/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 17:26:04 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 18:20:15 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../SocketAddress/IPv4Address.hpp"
# include "../RemoteServer.hpp"
# include "../LocalServerSocket.hpp"

int main(void)
{
    IPv4Address         ipv4(0, 8080);
    RemoteServer        server(ipv4, SOCK_STREAM, IPPROTO_TCP);

    LocalClientSocket   client(ipv4, SOCK_STREAM, IPPROTO_TCP);
    Connection*   client = server.accept();
    
    return (0);
}