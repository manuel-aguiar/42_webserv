/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogFile.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:28:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 10:39:37 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LogFile.hpp"
#include "../Globals/Globals.hpp"

LogFile::LogFile(const char* filename, Globals* globals) : _globals(globals)
{
    _fd = ::open(filename, O_CREAT | O_APPEND | O_CLOEXEC | O_NONBLOCK | O_RDWR, 0777);
    if (_fd == -1)
        throw std::runtime_error (std::string("Failed to start LogFile at: open(): ") + std::strerror(errno));
}

LogFile::~LogFile()
{
    ::close(_fd);
}

void    LogFile::setGlobals(Globals& globals)
{
    _globals = &globals;
}

void    LogFile::record(const std::string& entry)
{
    _globals->_clock->update();
    const char* clockBuf = _globals->_clock->get_FormatedTime();
    write(_fd, clockBuf, std::strlen(clockBuf));
    write(_fd, ": ", 2);
    write(_fd, entry.c_str(), entry.size());
    write(_fd, "\n", 1);
}

void    LogFile::record(const char* entry)
{
    _globals->_clock->update();
    const char* clockBuf = _globals->_clock->get_FormatedTime();
    write(_fd, clockBuf, std::strlen(clockBuf));
    write(_fd, ": ", 2);
    write(_fd, entry, std::strlen(entry));
    write(_fd, "\n", 1);
}