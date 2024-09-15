/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 09:14:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 09:54:17 by mmaria-d         ###   ########.fr       */
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


class DumbEpoll
{
    public:

        ~DumbEpoll()
        {
            for (size_t i = 0; i < _epoll.size(); i++)
                delete _epoll[i];
            _epoll.clear();
        }

        template <typename T>
        void add(UniquePtr<T> fd)
        {
            _epoll.push_back(fd.release());
        }

        FileDescriptor* operator[](size_t index)
        {
            return (_epoll[index]);
        }

    private:
        std::vector <FileDescriptor*> _epoll;
};


int main(void)
{
    try
    {
        DumbEpoll                                      epoll;
        
        IPv4Address                                     ipv4(0, 8080);
        ServerSocket                                    server(ipv4, SOCK_STREAM, IPPROTO_TCP);
        ClientSocket                                    client(ipv4, SOCK_STREAM, IPPROTO_TCP);
        
        server.bind();
        server.listen();
        client.connect();
        
        epoll.add(server.accept());
        
        epoll[0]->onWrite();
        
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