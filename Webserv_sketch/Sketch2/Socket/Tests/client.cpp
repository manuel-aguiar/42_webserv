/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 17:26:04 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 14:08:37 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../SocketAddress/IPv4Address.hpp"
# include "../ClientSocket.hpp"

int main(void)
{
    IPv4Address         ipv4(0, 8080);
    ClientSocket        client(ipv4, SOCK_STREAM, IPPROTO_TCP);

    client.connect();
    
    return (0);
}