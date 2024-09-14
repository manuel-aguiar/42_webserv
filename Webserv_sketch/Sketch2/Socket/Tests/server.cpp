/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 09:14:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/14 12:36:36 by mmaria-d         ###   ########.fr       */
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
        template <typename T>
        void add(UniquePtr<T> fd)
        {
            _epoll.push_back(UniquePtr<FileDescriptor>(dynamic_cast<FileDescriptor*>(fd.release())));
        }

        BorrowPtr<FileDescriptor> operator[](size_t index)
        {
            return (BorrowPtr<FileDescriptor>(_epoll[index]));
        }
    private:
        std::vector <UniquePtr<FileDescriptor> > _epoll;
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

        //BorrowPtr<ACommunicationSocket> borrow = epoll[0].lend();

        BorrowPtr<ACommunicationSocket> borrow(dynamic_cast<ACommunicationSocket*>(epoll[0].get()));
        borrow->send();
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