/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 09:14:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 14:07:35 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../SocketAddress/IPv4Address.hpp"
# include "../ServerSocket.hpp"
# include "../CommunicationSocket.hpp"

int main(void)
{
    IPv4Address                 ipv4(0, 8080);
    ServerSocket                server(ipv4, SOCK_STREAM, IPPROTO_TCP);
    server.bind();
    server.listen();            
    CommunicationSocket*         client = server.accept();
    
    return (0);
}