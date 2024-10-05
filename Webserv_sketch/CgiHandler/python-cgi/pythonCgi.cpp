/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pythonCgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 12:44:02 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/05 13:16:29 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pythonCgi.hpp"

PythonCgi::PythonCgi()
{
    _localDataPool = Nginx_MemoryPool::create(1024);
    _requestDataPool = Nginx_MemoryPool::create(4096);
    
    _baseEnv[PY_REQUEST_METHOD] = (char*)_localDataPool->allocate(sizeof("REQUEST_METHOD"), false);
    std::memcpy(_baseEnv[PY_REQUEST_METHOD], "REQUEST_METHOD", sizeof("REQUEST_METHOD"));
}


PythonCgi::~PythonCgi()
{
    _localDataPool->destroy();
    _requestDataPool->destroy();
}

void   PythonCgi::init()
{

}

void   PythonCgi::reset()
{
    _requestDataPool->reset();
}

PythonCgi::PythonCgi(const PythonCgi &other)
{
    *this = other;
}

PythonCgi &PythonCgi::operator=(const PythonCgi &other)
{
    if (this == &other)
        return (*this);
    return (*this);
}