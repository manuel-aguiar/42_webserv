/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pythonCgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 12:44:02 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/05 16:27:31 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pythonCgi.hpp"



PythonCgi::PythonCgi() : 
    _localDataPool(NULL), 
    _requestDataPool(NULL),
    _headersToEnum(std::less<char*>(), PY_CGIENV_COUNT),
    _enumToHeaders(std::less<PyCgiEnv>(), PY_CGIENV_COUNT)
{
    _localDataPool = Nginx_MemoryPool::create(1024);

    setupMapEntry("REQUEST_METHOD=", PY_REQUEST_METHOD);
    setupMapEntry("QUERY_STRING=", PY_QUERY_STRING);
    setupMapEntry("CONTENT_TYPE=", PY_CONTENT_TYPE);
    setupMapEntry("CONTENT_LENGTH=", PY_CONTENT_LENGTH);
    setupMapEntry("SCRIPT_NAME=", PY_SCRIPT_NAME);
    setupMapEntry("PATH_INFO=", PY_PATH_INFO);
    setupMapEntry("PATH_TRANSLATED=", PY_PATH_TRANSLATED);
    setupMapEntry("HTTP_COOKIE=", PY_HTTP_COOKIE);
    setupMapEntry("REMOTE_ADDR=", PY_REMOTE_ADDR);
    setupMapEntry("REMOTE_HOST=", PY_REMOTE_HOST);
    setupMapEntry("REMOTE_USER=", PY_REMOTE_USER);
    setupMapEntry("AUTH_TYPE=", PY_AUTH_TYPE);
    setupMapEntry("SERVER_NAME=", PY_SERVER_NAME);
    setupMapEntry("SERVER_PORT=", PY_SERVER_PORT);
    setupMapEntry("SERVER_PROTOCOL=", PY_SERVER_PROTOCOL);
    setupMapEntry("SERVER_SOFTWARE=", PY_SERVER_SOFTWARE);
    setupMapEntry("HTTP_USER_AGENT=", PY_HTTP_USER_AGENT);
    setupMapEntry("HTTP_REFERER=", PY_HTTP_REFERER);
    setupMapEntry("PYTHONPATH=", PY_PYTHON_PATH);


    _requestDataPool = Nginx_MemoryPool::create(4096);
}

void PythonCgi::setupMapEntry(const char *entry, PyCgiEnv enumerator)
{
    char* mapEntry;
    
    mapEntry = Nginx_MemoryPool::strdup(*_localDataPool, entry);
    _headersToEnum[mapEntry] = enumerator;
    _enumToHeaders[enumerator] = mapEntry;
}

void    PythonCgi::prepareCgi()
{

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