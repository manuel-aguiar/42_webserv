/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pythonCgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 12:44:02 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/06 13:04:22 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pythonCgi.hpp"
#include "../CgiRequest/CgiRequest.hpp"

PythonCgi::PythonCgi(const char* pythonBin) : 
    _localDataPool(Nginx_MemoryPool::create(1024)), 
    _requestDataPool(Nginx_MemoryPool::create(4096)),
    _localAllocator(*_localDataPool),
    _requestAllocator(*_requestDataPool),
    _headersToEnum(std::less<t_poolString>(), PY_CGIENV_COUNT),
    _enumToHeaders(std::less<PyCgiEnv>(), PY_CGIENV_COUNT),
    _pyBin(pythonBin, _localAllocator)
{

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

    _RequestEnvp[PY_CGIENV_COUNT] = NULL;
}

void PythonCgi::setupMapEntry(const char *entry, PyCgiEnv enumerator)
{
    std::pair<mapHeaderToEnum_Iter, bool> result;
    t_poolString* poolStrPtr;
    
    result = _headersToEnum.insert(std::make_pair(t_poolString(entry, _localAllocator), enumerator));
    poolStrPtr = const_cast<t_poolString*>(&result.first->first);
    _enumToHeaders.insert(std::make_pair(enumerator, poolStrPtr));
}

void    PythonCgi::printVariables()
{
    for (mapHeaderToEnum_Iter it = _headersToEnum.begin(); it != _headersToEnum.end(); ++it)
    {
        std::cout << "Key: " << it->first << " Value: " << it->second << std::endl;
    }
}

void    PythonCgi::printEnumerators()
{
    for (mapEnumToHeader_Iter it = _enumToHeaders.begin(); it != _enumToHeaders.end(); ++it)
    {
        std::cout << "Key: " << it->first << " Value: " << *it->second << std::endl;
    }
}

CgiRequest PythonCgi::prepareCgi(const char* scriptPath)
{

    //_requestDataPool->reset(); // not workiiiiiiiing
    
    for (mapHeaderToEnum_Iter it = _headersToEnum.begin(); it != _headersToEnum.end(); ++it)
    {
        t_poolString* str = (t_poolString*)_requestDataPool->allocate(sizeof(t_poolString), true);
        new (str) t_poolString(it->first.c_str(), _requestAllocator);
        _RequestEnvp[it->second] = const_cast<char*>(str->c_str());
    }
    
    _RequestArgv = (char **) _requestDataPool->allocate(3 * sizeof(char*), true);
    _RequestArgv[0] = const_cast<char*>(_pyBin.c_str());
    _RequestArgv[1] = const_cast<char*>(scriptPath);
    _RequestArgv[2] = NULL;

    return (CgiRequest((const char**)_RequestArgv, (const char**)_RequestEnvp, (const char*)NULL));
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




//private

PythonCgi::PythonCgi(const PythonCgi &other) : 
    _localDataPool(other._localDataPool), 
    _requestDataPool(other._requestDataPool),
    _localAllocator(other._localAllocator),
    _requestAllocator(other._requestAllocator),
    _headersToEnum(other._headersToEnum),
    _enumToHeaders(other._enumToHeaders),
    _pyBin(other._pyBin)
{
    *this = other;
}


PythonCgi &PythonCgi::operator=(const PythonCgi &other)
{
    if (this == &other)
        return (*this);
    return (*this);
}