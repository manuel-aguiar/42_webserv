/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pythonCgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 12:44:02 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/12 10:50:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pythonCgi.hpp"
#include "../CgiLiveRequest/CgiLiveRequest.hpp"

PythonCgi::PythonCgi(const char* pythonBin) :
    m_localDataPool(Nginx_MemoryPool::create(1024)),
    m_localAllocator(m_localDataPool),
    m_headersToEnum(std::less<t_poolString>(), PY_CGIENV_COUNT),
    m_enumToHeaders(std::less<PyCgiEnv>(), PY_CGIENV_COUNT),
    m_pyBin(pythonBin, m_localAllocator)
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
}

void PythonCgi::setupMapEntry(const char *entry, PyCgiEnv enumerator)
{
    std::pair<mapHeaderToEnum_Iter, bool> result;
    t_poolString* poolStrPtr;

    result = m_headersToEnum.insert(std::make_pair(t_poolString(entry, m_localAllocator), enumerator));
    poolStrPtr = const_cast<t_poolString*>(&result.first->first);
    m_enumToHeaders.insert(std::make_pair(enumerator, poolStrPtr));
}

void    PythonCgi::printVariables()
{
    for (mapHeaderToEnum_Iter it = m_headersToEnum.begin(); it != m_headersToEnum.end(); ++it)
    {
        std::cout << "Key: " << it->first << " Value: " << it->second << std::endl;
    }
}

void    PythonCgi::printEnumerators()
{
    for (mapEnumToHeader_Iter it = m_enumToHeaders.begin(); it != m_enumToHeaders.end(); ++it)
    {
        std::cout << "Key: " << it->first << " Value: " << *it->second << std::endl;
    }
}

void PythonCgi::prepareCgi(CgiLiveRequest& req, const char* scriptPath)
{
    req.m_envp = (char**) req.m_requestDataPool->allocate((PY_CGIENV_COUNT + 1) * sizeof(char*), true);
    req.m_envp[PY_CGIENV_COUNT] = NULL;

    for (mapHeaderToEnum_Iter it = m_headersToEnum.begin(); it != m_headersToEnum.end(); ++it)
    {
        t_poolString* str = (t_poolString*)req.m_requestDataPool->allocate(sizeof(t_poolString), true);
        new (str) t_poolString(it->first.c_str(), req.m_strAlloc);
        req.m_envp[it->second] = const_cast<char*>(str->c_str());
    }

    req.m_argv = (char**) req.m_requestDataPool->allocate(3 * sizeof(char*), true);
    req.m_argv[0] = const_cast<char*>(m_pyBin.c_str());
    req.m_argv[1] = const_cast<char*>(scriptPath);
    req.m_argv[2] = NULL;
}

PythonCgi::~PythonCgi()
{
    m_localDataPool->destroy();
}

void   PythonCgi::init()
{

}





//private

PythonCgi::PythonCgi(const PythonCgi &other) :
    m_localDataPool(other.m_localDataPool),
    m_localAllocator(other.m_localAllocator),
    m_headersToEnum(other.m_headersToEnum),
    m_enumToHeaders(other.m_enumToHeaders),
    m_pyBin(other.m_pyBin)
{
    *this = other;
}


PythonCgi &PythonCgi::operator=(const PythonCgi &other)
{
    if (this == &other)
        return (*this);
    return (*this);
}
