/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:25:09 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 11:25:23 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "FileDescriptor.hpp"

FileDescriptor::FileDescriptor() : _fd(-1), _closeOnDestroy(false) {}
FileDescriptor::FileDescriptor(int fileDescriptor, bool closeOnDestroy) : 
    _fd(fileDescriptor),
    _closeOnDestroy(closeOnDestroy) {}

FileDescriptor::~FileDescriptor() {}

int FileDescriptor::getFd() const
{
    return (_fd);
}

void FileDescriptor::setCloseOnDestroy(const bool closeOnDestroy)
{
    _closeOnDestroy = closeOnDestroy;
}

bool FileDescriptor::getCloseOnDestroy() const
{
    return (_closeOnDestroy);
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

void    FileDescriptor::close()
{
    if (_fd != -1)
        ::close(_fd);
    _fd = -1;
}

FileDescriptor::FileDescriptor(const FileDescriptor& hardCopy)  : _fd(hardCopy._fd), _closeOnDestroy(hardCopy._closeOnDestroy) {(void)hardCopy;}
FileDescriptor& FileDescriptor::operator=(const FileDescriptor& hardAssign)
{
    if (this == &hardAssign)
        return (*this);
    _fd = hardAssign._fd;
    _closeOnDestroy = hardAssign._closeOnDestroy;
    return (*this);
}