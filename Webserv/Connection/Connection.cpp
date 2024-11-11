/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/11 13:48:37 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "../Globals/Globals.hpp"
#include "../ListeningSocket/ListeningSocket.hpp"

Connection::Connection(Globals* globals) :
    m_memPool(NULL),
    m_globals(globals)
{

}

Connection::~Connection()
{

}

void    Connection::init()
{
    m_memPool = Nginx_MemoryPool::create(4096, 10);
}

void    Connection::reset()
{
    m_sockfd = -1;
    m_listener = NULL;
    m_memPool->reset();
}

const char* response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 143\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html>\r\n"
        "  <head>\r\n"
        "    <title>Welcome</title>\r\n"
        "  </head>\r\n"
        "  <body>\r\n"
        "    <h1>Welcome to My Server!</h1>\r\n"
        "    <p>This is a simple HTTP response.</p>\r\n"
        "  </body>\r\n"
        "</html>\r\n";

void    Connection::read()
{
    //m_globals->logStatus("RUUUUUAD");

    char buf[1024];

    int bytes = ::read(m_sockfd, buf, 1024);
    if (bytes == -1)
    {
        std::cout << "                  read(): " + std::string(std::strerror(errno)) << std::endl;
        m_listener->closeConnection(this);
        return ;
    }
    if (bytes == 0)
    {
        m_listener->closeConnection(this);
        std::cout << "                  read(): gracefullexit" << std::endl;
        return ;
    }
    buf[bytes] = '\0';
    std::cout << "                  read(): " + std::string(buf) << std::endl;

    write();

}

void    Connection::write()
{
    std::cout << "conenction fd " << m_sockfd << " writing" << std::endl;
    ::write(m_sockfd, response, std::strlen(response));
    std::cout << "conenction fd " << m_sockfd << " finished writing" << std::endl;
    //m_listener->closeConnection(this);
}



// no copies, as usual
Connection::Connection(const Connection& other) :
    m_memPool(other.m_memPool),
    m_globals(other.m_globals)
{
    (void)other;
}
Connection& Connection::operator=(const Connection& other)
{
    if (this == &other)
        return (*this);
    m_memPool = other.m_memPool;
    m_globals = other.m_globals;
    return (*this);
}
