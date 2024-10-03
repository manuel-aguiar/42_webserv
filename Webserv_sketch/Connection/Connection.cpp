/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 10:51:36 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"


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
    _listener = NULL;
    _memPool->reset();
}

void    Connection::read() {}
void    Connection::write() {}



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
