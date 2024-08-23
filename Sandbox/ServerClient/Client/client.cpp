/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 08:42:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/23 15:34:54 by mmaria-d         ###   ########.fr       */
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
# define LOCALHOST "127.0.0.1"

# define REQUEST "Hello Server!"

/*
struct sockaddr_in {
    short            sin_family;   // Address family (AF_INET)
    unsigned short   sin_port;     // Port number (in network byte order)
    struct in_addr   sin_addr;     // Internet address (struct in_addr)
};
*/

int main()
{
    int                 connection;
    struct sockaddr_in  connAddress;
    int                 bytesRead;
    char                readBuff[256];

    std::memset(&connAddress, 0, sizeof(connAddress));
    connAddress.sin_family = AF_INET;
    connAddress.sin_addr.s_addr = inet_addr(LOCALHOST);
    connAddress.sin_port = htons(PORT);
    
    connection = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    std::cout << "Sending Request...." << std::endl;

    if (connect(connection, (struct sockaddr *)&connAddress, sizeof(connAddress)) == -1)
    {
        std::cerr << "connect(): " << std::string (std::strerror(errno)) << std::endl;
        close(connection);
        return (EXIT_FAILURE);
    }

    if (write(connection, REQUEST, std::strlen(REQUEST) + 1) == -1)
    {
        std::cerr << "write(): " << std::string (std::strerror(errno)) << std::endl;
        close(connection);
        return (EXIT_FAILURE);        
    }
    
    while (true)
    {
        std::memset(&readBuff, 0, sizeof(readBuff));
        bytesRead = read(connection, readBuff, sizeof(readBuff) - 1);
        if (bytesRead == -1)
        {
            std::cerr << "read(): " << std::string (std::strerror(errno)) << std::endl;
            close(connection);
            return (EXIT_FAILURE);
        }
        if (bytesRead > 0)
            std::cout << readBuff;
        else
            break ;
    }

    std::cout << std::endl;
    
    close(connection);
    return (EXIT_SUCCESS);
}