/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 09:14:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 14:20:59 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
    clear && c++ -Wall -Wextra -Werror $(find . -name "*.cpp") ../FileDescriptor/FileDescriptor.cpp ../Exceptions/ParameterException.cpp -o sockets
*/

# include "../SocketAddress/IPv4Address.hpp"
# include "../ServerSocket.hpp"
# include "../CommunicationSocket.hpp"

# include <iostream>

int main(void)
{
    try
    {
        IPv4Address                 ipv4(0, 8080);
        ServerSocket                server1(ipv4, SOCK_STREAM, IPPROTO_TCP);
        ServerSocket                server2(ipv4, SOCK_STREAM, IPPROTO_TCP);
        
        if (server1 == server2)
            std::cout << "server1 and server2 are equal" << std::endl;
        else
            std::cout << "server1 and server2 are not equal" << std::endl;
        return (0);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return (1);
    }
    return (0);
}