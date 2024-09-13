/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 09:14:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 19:23:00 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
    clear && c++ -Wall -Wextra -Werror $(find . -name "*.cpp") ../FileDescriptor/FileDescriptor.cpp ../Exceptions/ParameterException.cpp -o sockets

    valgrind --track-fds=yes ./sockets
*/

# include "../SocketAddress/IPv4Address.hpp"
# include "../ServerSocket.hpp"
# include "../ClientSocket.hpp"
# include "../CommunicationSocket.hpp"

# include <iostream>
# include <map>
# include <vector>


int main(void)
{
    try
    {
        std::vector<UniquePtr<ACommunicationSocket> >   connections;
        
        IPv4Address                                     ipv4(0, 8080);
        ServerSocket                                    server(ipv4, SOCK_STREAM, IPPROTO_TCP);
        ClientSocket                                    client(ipv4, SOCK_STREAM, IPPROTO_TCP);
        
        server.bind();
        server.listen();
        client.connect();
        
        connections.push_back(server.accept());
        connections[0]->send();

        client.receive();
        
        return (0);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return (1);
    }
    return (0);
}