/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogFileThreadSafeThreadSafe.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:39:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 10:39:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "LogFileThreadSafe.hpp"

LogFileThreadSafe::LogFileThreadSafe(const char* filename)
{
    m_fd = open(filename, O_CREAT | O_APPEND | O_NONBLOCK | O_CLOEXEC | O_RDWR, S_IRWXG);
    if (m_fd == -1)
        throw std::runtime_error (std::string("Failed to start LogFileThreadSafe at: open(): ") + std::strerror(errno));
    if (pthread_mutex_init(&m_writeLock, NULL))
        throw std::runtime_error (std::string("Failed to start LogFileThreadSafe at: pthread_mutex_init(): ") + std::strerror(errno));
}

LogFileThreadSafe::~LogFileThreadSafe()
{
    close(m_fd);
    pthread_mutex_destroy(&m_writeLock);
}

void    LogFileThreadSafe::record(const std::string& entry)
{
    if (!pthread_mutex_lock(&m_writeLock))
    {
        write(m_fd, entry.c_str(), entry.size() + 1);
        pthread_mutex_unlock(&m_writeLock);
    }
}

void    LogFileThreadSafe::record(const char* entry)
{
    write(m_fd, entry, std::strlen(entry));
}