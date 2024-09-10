/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 09:14:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 18:20:20 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../SocketAddress/IPv4Address.hpp"
# include "../RemoteClient.hpp"
# include "../LocalServerSocket.hpp"

int main(void)
{
    IPv4Address                 ipv4(0, 8080);
    LocalServerSocket           server(ipv4, SOCK_STREAM, IPPROTO_TCP, 100);
    RemoteClient*               client = server.acceptConnection();
    
    return (0);
}