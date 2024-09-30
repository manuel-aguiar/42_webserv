/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/30 15:05:05 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

Connection::Connection() :
    _connectionAlloc(*Nginx_MemoryPool::create(4096, 1))
{

}

Connection::~Connection()
{
    _connectionAlloc.destroy();
}

void    Connection::reset()
{
    _listener = NULL;
    _connectionAlloc.reset();
}

void    Connection::read() {}
void    Connection::write() {}

// no copies, as usual
Connection::Connection(const Connection& other) :
    _connectionAlloc(other._connectionAlloc)
{
    (void)other;
}
Connection& Connection::operator=(const Connection& other)
{
    (void)other;
    return (*this);
}