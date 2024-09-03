/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getaddrinfo.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 15:33:06 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/30 16:09:03 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <sys/types.h>
# include <netdb.h>

# include <cstring>
# include <iostream>


/*
    getaddrinfo 
*/

int main(int ac, char **av)
{
    struct addrinfo*        placeResult;
    struct addrinfo         hint;
    int                     addrinfoStatus;
    int                     addrcount = 0;

    if (ac != 3)
    {
        std::cout << "wrong number of args" << std::endl;
        return (0);
    }

    std::memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_DGRAM;
    hint.ai_protocol = 0;
    
    addrinfoStatus = getaddrinfo(av[1], av[2], &hint, &placeResult);
    if (addrinfoStatus != 0)
    {
        std::cerr << "getaddrinfo(): " + std::string(gai_strerror(addrinfoStatus)) << std::endl;
        return (1);
    }    
    
    for (struct addrinfo* iter = placeResult; iter != NULL; iter = iter->ai_next)
        addrcount++;
    freeaddrinfo(placeResult);
    std::cout << "total possible addresses to bind that fit criteria: " << addrcount << std::endl;
    return (0);

}
