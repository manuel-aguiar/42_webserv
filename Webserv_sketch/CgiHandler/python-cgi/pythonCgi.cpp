/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pythonCgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 12:44:02 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/05 14:05:08 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pythonCgi.hpp"

PythonCgi::PythonCgi() : 
    _localDataPool(NULL), 
    _requestDataPool(NULL),
    _headersToEnum(std::less<char*>(), PY_CGIENV_COUNT)
{
    _localDataPool = Nginx_MemoryPool::create(1024);
    
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "REQUEST_METHOD=")] = PY_REQUEST_METHOD;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "QUERY_STRING=")] = PY_QUERY_STRING;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "CONTENT_TYPE=")] = PY_CONTENT_TYPE;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "CONTENT_LENGTH=")] = PY_CONTENT_LENGTH;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "SCRIPT_NAME=")] = PY_SCRIPT_NAME;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "PATH_INFO=")] = PY_PATH_INFO;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "PATH_TRANSLATED=")] = PY_PATH_TRANSLATED;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "HTTP_COOKIE=")] = PY_HTTP_COOKIE;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "REMOTE_ADDR=")] = PY_REMOTE_ADDR;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "REMOTE_HOST=")] = PY_REMOTE_HOST;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "REMOTE_USER=")] = PY_REMOTE_USER;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "AUTH_TYPE=")] = PY_AUTH_TYPE;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "SERVER_NAME=")] = PY_SERVER_NAME;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "SERVER_PORT=")] = PY_SERVER_PORT;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "SERVER_PROTOCOL=")] = PY_SERVER_PROTOCOL;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "SERVER_SOFTWARE=")] = PY_SERVER_SOFTWARE;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "HTTP_USER_AGENT=")] = PY_HTTP_USER_AGENT;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "HTTP_REFERER=")] = PY_HTTP_REFERER;
    _headersToEnum[Nginx_MemoryPool::strdup(*_localDataPool, "PYTHONPATH=")] = PY_PYTHON_PATH;


    _requestDataPool = Nginx_MemoryPool::create(4096);
}

void    PythonCgi::prepareCgi()
{
    for (PyCgiEnvMap_Iter it = _headersToEnum.begin(); it != _headersToEnum.end(); it++)
    {
        char* envVar = Nginx_MemoryPool::strdup(*_requestDataPool, it->first);
        char* envVal = Nginx_MemoryPool::allocate(*_requestDataPool, 1024, false);
        std::cout << "Enter value for " << it->first << std::endl;
        std::cin >> envVal;
        std::cout << "Value for " << it->first << " is " << envVal << std::endl;
        setenv(envVar, envVal, 1);
    }
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