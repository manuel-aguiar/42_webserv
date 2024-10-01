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
    _fd = open(filename, O_CREAT | O_APPEND | O_CLOEXEC | O_RDWR, 777);
    if (_fd == -1)
        throw std::runtime_error (std::string("Failed to start LogFile at: open(): ") + std::strerror(errno));
}

LogFile::~LogFile()
{
    close(_fd);
}

void    LogFile::setGlobals(Globals& globals)
{
    _globals = &globals;
}

void    LogFile::record(const std::string& entry)
{
    //get the time with the clock
    write(_fd, entry.c_str(), entry.size() + 1);
}

void    LogFile::record(const char* entry)
{
    //get the time with the clock
    write(_fd, entry, std::strlen(entry));
}