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

LogFileThreadSafe::LogFileThreadSafe()
{
    _fd = open("LogFileThreadSafeFile.txt", O_CREAT | O_APPEND | O_NONBLOCK | O_CLOEXEC | O_RDWR, S_IRWXG);
    if (_fd == -1)
        throw std::runtime_error (std::string("Failed to start LogFileThreadSafe at: open(): ") + std::strerror(errno));
    if (pthread_mutex_init(&_writeLock, NULL))
        throw std::runtime_error (std::string("Failed to start LogFileThreadSafe at: pthread_mutex_init(): ") + std::strerror(errno));

}

LogFileThreadSafe::~LogFileThreadSafe()
{
    close(_fd);
    pthread_mutex_destroy(&_writeLock);
}

void    LogFileThreadSafe::record(const std::string& entry)
{
    if (!pthread_mutex_lock(&_writeLock))
    {
        write(_fd, entry.c_str(), entry.size() + 1);
        pthread_mutex_unlock(&_writeLock);
    }
}