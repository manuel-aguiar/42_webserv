/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogStreamThreadSafeThreadSafe.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:28:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 10:39:37 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LogStreamThreadSafe.hpp"

LogStreamThreadSafe::LogStreamThreadSafe(std::ostream& stream) : m_stream(stream)
{
    if (pthread_mutex_init(&m_writeLock, NULL))
        throw std::runtime_error (std::string("Failed to start LogThreadSafe at: pthread_mutex_init(): ") + std::strerror(errno));
}

LogStreamThreadSafe::~LogStreamThreadSafe()
{
    pthread_mutex_destroy(&m_writeLock);
}

void    LogStreamThreadSafe::record(const std::string& entry)
{
    if (!pthread_mutex_lock(&m_writeLock))
    {
        m_stream << entry;
        pthread_mutex_unlock(&m_writeLock);
    }
}


void    LogStreamThreadSafe::record(const char* entry)
{
    if (!pthread_mutex_lock(&m_writeLock))
    {
        m_stream << entry;
        pthread_mutex_unlock(&m_writeLock);
    }
}
