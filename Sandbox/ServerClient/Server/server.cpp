/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 08:42:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/23 15:26:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// socket(), send(), recv(), accept(), connect(), bind(), listen()
# include <sys/socket.h>

//IPPROTO MACROS and more
# include <netinet/in.h>


// byte order converters
# include <arpa/inet.h>

# include <unistd.h>


# include <cstring>
# include <iostream>

# define PORT 8080
# define MAX_CONNECTIONS 10
# define RESPONSE "Hello Client!"

/*
struct sockaddr_in {
    short            sin_family;   // Address family (AF_INET)
    unsigned short   sin_port;     // Port number (in network byte order)
    struct in_addr   sin_addr;     // Internet address (struct in_addr)
};
*/

int main()
{
    int                 listener;
    int     	        connection;
    struct sockaddr_in  listAddress;
    struct sockaddr_in  connAddress;
    socklen_t           connAddrLen;
    int                 bytesRead;
    char                readBuff[256];

    std::memset(&listAddress, 0, sizeof(listAddress));
    listAddress.sin_family = AF_INET;
    listAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    listAddress.sin_port = htons(PORT);
    
    listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (bind(listener, (struct sockaddr*)&listAddress, sizeof(listAddress)) == -1)
    {
        std::cerr << "bind(): " << std::string (std::strerror(errno)) << std::endl;
        close(listener);
        return (EXIT_FAILURE);
    }
    if (listen(listener, MAX_CONNECTIONS) == -1)
    {
        std::cerr << "listen(): " << std::string (std::strerror(errno)) << std::endl;
        close(listener);
        return (EXIT_FAILURE);
    }


    connAddrLen = sizeof(connAddress);
    while(true)
    {
        std::cout << "  Server: Waiting for connections...." << std::endl;
        connection = accept(listener, (struct sockaddr*)&connAddress, &connAddrLen);
        if (connection == -1)
        {
            std::cerr << "accept(): " << std::string (std::strerror(errno)) << std::endl;
            close(listener);
            return (EXIT_FAILURE);
        }
        std::cout << "  Server: Connection received" << std::endl;
        while (true)
        {
            std::memset(readBuff, 0, sizeof(readBuff));
            bytesRead = read(connection, readBuff, sizeof(readBuff) - 1);

            if (bytesRead == -1)
            {
                std::cerr << "read(): " << std::strerror(errno) << std::endl;
                close(connection);
                close(listener);
                return EXIT_FAILURE;
            }
            else if (bytesRead == 0)
            {
                std::cout << "Client closed the connection" << std::endl;
                break;
            }
            else
            {
                std::cout << readBuff;
                if (readBuff[bytesRead] == '\0')
                    break ;
            }
        }
        if (write(connection, RESPONSE, std::strlen(RESPONSE) + 1) == -1)
        {
            std::cerr << "write(): " << std::string (std::strerror(errno)) << std::endl;
            close(connection);
            close(listener);
            return (EXIT_FAILURE);
        }
        std::cout << std::endl;
        close(connection);
    }
    

    close(listener);
    return (EXIT_SUCCESS);
}