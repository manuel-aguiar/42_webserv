/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/11 15:11:40 by codespace        ###   ########.fr       */
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

#include <sys/stat.h>

std::string int_to_string(int value) {
    // Handle zero case
    if (value == 0) {
        return "0";
    }

    bool isNegative = value < 0;
    std::string result;

    // Process each digit and prepend to the result string
    while (value != 0) {
        int digit = (isNegative ? -(value % 10) : (value % 10));
        result = static_cast<char>('0' + digit) + result;
        value /= 10;
    }

    // Add negative sign if needed
    if (isNegative) {
        result = "-" + result;
    }

    return result;
}

void    Connection::write()
{
	char buffer [100];

	const char* path = "/workspaces/42_webserv/Websites/Website1/index.html";
	const char* response = 	"HTTP/1.1 200 OK\r\n"
        					"Content-Type: text/html\r\n"
							"Connection: close\r\n";
    ::write(m_sockfd, response, std::strlen(response));

	int fd = ::open(path, O_RDONLY);
	struct stat fileStat;
	if (stat(path, &fileStat) != 0)
	{
		std::cout << "couldnt reach file" << std::endl;
		return ;
	}

	int size = fileStat.st_size;
	std::string cenas = "Content-Length: " + int_to_string(size) + "\r\n\r\n";
	::write(m_sockfd, cenas.c_str(), cenas.size());
	std::cout << "conenction fd " << m_sockfd << " started writing" << std::endl;

	while (1)
	{
		int bytes = ::read(fd, buffer, 100);

		::write(m_sockfd, buffer, bytes);
		if (bytes < 100)
			break;
	}

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
