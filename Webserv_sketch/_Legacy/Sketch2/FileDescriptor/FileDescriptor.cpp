/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:25:09 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:10:56 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "FileDescriptor.hpp"

FileDescriptor::FileDescriptor() : m_fd(-1) {}
FileDescriptor::FileDescriptor(int fileDescriptor) : m_fd(fileDescriptor) {}

FileDescriptor::~FileDescriptor() {}

int FileDescriptor::getFd() const
{
    return (m_fd);
}

bool FileDescriptor::operator==(const int other) const
{
    return (m_fd == other);
}

bool FileDescriptor::operator==(const FileDescriptor& other) const
{
    return (m_fd == other.m_fd);
}

bool FileDescriptor::operator<(const int other) const
{
    return (m_fd < other);
}

bool FileDescriptor::operator<(const FileDescriptor& other) const
{
    return (m_fd < other.m_fd);
}

int FileDescriptor::operator()() const
{
    return (m_fd);
}

bool    FileDescriptor::setCloseOnExec_NonBlocking()
{
    return (_addFlags(O_NONBLOCK | O_CLOEXEC));
}

bool    FileDescriptor::_addFlags(const int flags)
{
    int currentFlags = ::fcntl(m_fd, F_GETFL, 0);
    if (currentFlags == -1)
        return (false);
    return (::fcntl(m_fd, F_SETFL, currentFlags | flags) != -1);
}

bool    FileDescriptor::redirectTo(const int newFd)
{
    return (::dup2(m_fd, newFd) != -1);
}

bool   FileDescriptor::redirectTo(const FileDescriptor& newFd)
{
    return (::dup2(m_fd, newFd.m_fd) != -1);
}

void    FileDescriptor::close()
{
    if (m_fd != -1)
        ::close(m_fd);
    m_fd = -1;
}

FileDescriptor::FileDescriptor(const FileDescriptor& hardCopy)  : m_fd(hardCopy.m_fd) {(void)hardCopy;}
FileDescriptor& FileDescriptor::operator=(const FileDescriptor& hardAssign)
{
    if (this == &hardAssign)
        return (*this);
    m_fd = hardAssign.m_fd;
    return (*this);
}