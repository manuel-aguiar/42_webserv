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

LogFile::LogFile(const char* filename, Globals* globals) : m_globals(globals)
{
    m_fd = ::open(filename, O_CREAT | O_APPEND | O_CLOEXEC | O_NONBLOCK | O_RDWR, 0777);
    if (m_fd == -1)
        throw std::runtime_error (std::string("Failed to start LogFile at: open(): ") + std::strerror(errno));
}

LogFile::~LogFile()
{
    ::close(m_fd);
}

void    LogFile::setGlobals(Globals& globals)
{
    m_globals = &globals;
}

void    LogFile::record(const std::string& entry)
{
    m_globals->m_clock->update();
    const char* clockBuf = m_globals->m_clock->get_FormatedTime();
    write(m_fd, clockBuf, std::strlen(clockBuf));
    write(m_fd, ": ", 2);
    write(m_fd, entry.c_str(), entry.size());
    write(m_fd, "\n", 1);
}

void    LogFile::record(const char* entry)
{
    m_globals->m_clock->update();
    const char* clockBuf = m_globals->m_clock->get_FormatedTime();
    write(m_fd, clockBuf, std::strlen(clockBuf));
    write(m_fd, ": ", 2);
    write(m_fd, entry, std::strlen(entry));
    write(m_fd, "\n", 1);
}