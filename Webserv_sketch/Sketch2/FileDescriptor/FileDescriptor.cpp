/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:25:09 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 15:09:07 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "FileDescriptor.hpp"

FileDescriptor::FileDescriptor() : _fd(-1) {}
FileDescriptor::FileDescriptor(int fileDescriptor) : _fd(fileDescriptor) {}

FileDescriptor::~FileDescriptor() {}

int FileDescriptor::getFd() const
{
    return (_fd);
}

bool FileDescriptor::operator==(const int other) const
{
    return (_fd == other);
}

bool FileDescriptor::operator==(const FileDescriptor& other) const
{
    return (_fd == other._fd);
}

bool FileDescriptor::operator<(const int other) const
{
    return (_fd < other);
}

bool FileDescriptor::operator<(const FileDescriptor& other) const
{
    return (_fd < other._fd);
}

int FileDescriptor::operator()() const
{
    return (_fd);
}

bool    FileDescriptor::setCloseOnExec_NonBlocking()
{
    return (_addFlags(O_NONBLOCK | O_CLOEXEC));
}

bool    FileDescriptor::_addFlags(const int flags)
{
    int currentFlags = ::fcntl(_fd, F_GETFL, 0);
    if (currentFlags == -1)
        return (false);
    return (::fcntl(_fd, F_SETFL, currentFlags | flags) != -1);
}

bool    FileDescriptor::redirectTo(const int newFd)
{
    return (::dup2(_fd, newFd) != -1);
}

bool   FileDescriptor::redirectTo(const FileDescriptor& newFd)
{
    return (::dup2(_fd, newFd._fd) != -1);
}

void    FileDescriptor::close()
{
    if (_fd != -1)
        ::close(_fd);
    _fd = -1;
}

FileDescriptor::FileDescriptor(const FileDescriptor& hardCopy)  : _fd(hardCopy._fd) {(void)hardCopy;}
FileDescriptor& FileDescriptor::operator=(const FileDescriptor& hardAssign)
{
    if (this == &hardAssign)
        return (*this);
    _fd = hardAssign._fd;
    return (*this);
}