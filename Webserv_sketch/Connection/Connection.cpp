/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/04 16:29:09 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "../Globals/Globals.hpp"
#include "../ListeningSocket/ListeningSocket.hpp"

Connection::Connection(Globals* globals) :
    _memPool(NULL),
    _globals(globals)
{
    
}

Connection::~Connection()
{

}

void    Connection::init()
{
    _memPool = Nginx_MemoryPool::create(4096, 10);
}

void    Connection::reset()
{
    _sockfd = -1;
    _listener = NULL;
    _memPool->reset();
}

const char* response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 127\r\n"
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
    //_globals->_logFile->record("RUUUUUAD");

    char buf[1024];

    int bytes = ::read(_sockfd, buf, 1024);
    if (bytes == -1)
    {
        std::cout << "                  read(): " + std::string(std::strerror(errno)) << std::endl;
        _listener->closeConnection(this);
        return ;
    }
    if (bytes == 0)
    {
        _listener->closeConnection(this);
        std::cout << "                  read(): gracefullexit" << std::endl;
        return ;
    }
    buf[bytes] = '\0';
    std::cout << "                  read(): " + std::string(buf) << std::endl;

    write();

}

void    Connection::write()
{ 
    std::cout << "conenction fd " << _sockfd << " writing" << std::endl;
    ::write(_sockfd, response, std::strlen(response)); 
    std::cout << "conenction fd " << _sockfd << " finished writing" << std::endl;
    //_listener->closeConnection(this);
}



// no copies, as usual
Connection::Connection(const Connection& other) :
    _memPool(other._memPool),
    _globals(other._globals)
{
    (void)other;
}
Connection& Connection::operator=(const Connection& other)
{
    if (this == &other)
        return (*this);
    _memPool = other._memPool;
    _globals = other._globals;
    return (*this);
}
