/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 09:14:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 17:23:45 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
   clear && c++ -Wall -Wextra -Werror $(find . -name "*.cpp") ../FileDescriptor/FileDescriptor.cpp ../FileDescriptorManager/FileDescriptorManager.cpp ../Exceptions/ParameterException.cpp ../EventPoll/EventManager.cpp -o sockets

    valgrind --track-fds=yes ./sockets
*/

# include "../SocketAddress/IPv4Address.hpp"
# include "../ServerSocket.hpp"
# include "../ClientSocket.hpp"
# include "../CommunicationSocket.hpp"
# include "../../FileDescriptorManager/FileDescriptorManager.hpp"


# include <iostream>
# include <map>
# include <vector>


int main(void)
{
    try
    {
        FileDescriptorManager                           fdManager;
        
        IPv4Address                                     ipv4(0, 8080);
        UniquePtr<ServerSocket<IPv4Address> >           server = new ServerSocket<IPv4Address> (fdManager, ipv4, SOCK_STREAM, IPPROTO_TCP);
        UniquePtr<ClientSocket<IPv4Address> >           client = new ClientSocket<IPv4Address> (ipv4, SOCK_STREAM, IPPROTO_TCP);
        
        int serverFd = server->getFd();
        server->bind();
        server->listen();
        client->connect();

        fdManager.addFileDescriptor(server, true);
        fdManager.addFileDescriptor(client, false);


        fdManager.getFileDescriptor(serverFd)->onRead();
        
        return (0);

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return (1);
    }
    return (0);
}