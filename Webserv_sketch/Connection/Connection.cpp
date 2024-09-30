/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/30 12:20:51 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

Connection::Connection() :
    _pool(*Nginx_MemoryPool::create(4096, 1))
{

}

Connection::~Connection()
{
    _pool.destroy();
}

void    Connection::reset()
{
    _listener = NULL;
    _pool.reset();
}

void    Connection::read() {}
void    Connection::write() {}

// no copies, as usual
Connection::Connection(const Connection& other) :
    _pool(other._pool)
{
    (void)other;
}
Connection& Connection::operator=(const Connection& other)
{
    (void)other;
    return (*this);
}