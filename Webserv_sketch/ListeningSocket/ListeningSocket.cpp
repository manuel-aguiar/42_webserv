/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:52:40 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/30 10:22:02 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket()
{
}

ListeningSocket::~ListeningSocket()
{
}

int    ListeningSocket::open()
{
    int options;

    _sockfd = ::socket(_addr->sa_family, _socktype, _proto);
    
    if (_sockfd == -1)
    {
        std::cerr << "socket(): " + std::string(strerror(errno)) << std::endl;
        return (0);
    }
    
    #ifdef SO_REUSEPORT
        options = SO_REUSEADDR | SO_REUSEPORT;
    #else
        options = SO_REUSEADDR;
    #endif

    if (::setsockopt(_sockfd, SOL_SOCKET, options, &options, sizeof(options)) == -1)
    {
        std::cerr << "setsockopt(): " + std::string(strerror(errno)) << std::endl;
        return (0);
    }   

    if (!bind() || !listen())
        return (0);
    return (1);
}

int    ListeningSocket::bind()
{
    if (::bind(_sockfd, _addr, _addrlen) == -1)
    {
        std::cerr << "bind(): " + std::string(strerror(errno)) << std::endl;
        return (0);
    }
    return (1);
}

int    ListeningSocket::listen()
{
    if (::listen(_sockfd, _backlog) == -1)
    {
        std::cerr << "listen(): " + std::string(strerror(errno)) << std::endl;
        return (0);
    }
    return (1);
}

void    ListeningSocket::accept()
{
}

void    ListeningSocket::close()
{
}

