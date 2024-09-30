/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:52:40 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/30 10:14:19 by mmaria-d         ###   ########.fr       */
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
    _sockfd = socket(_addr->sa_family, _socktype, _proto);
    
    if (_sockfd == -1)
    {
        std::cerr << "socket(): " + std::string(strerror(errno)) << std::endl;
        return (1);
    }

    return (0);
}

void    ListeningSocket::bind()
{
}

void    ListeningSocket::listen()
{
}

void    ListeningSocket::accept()
{
}

void    ListeningSocket::close()
{
}

