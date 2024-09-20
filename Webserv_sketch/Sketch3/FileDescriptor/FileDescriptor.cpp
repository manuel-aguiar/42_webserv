/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:25:09 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 12:36:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "FileDescriptor.hpp"

FileDescriptor::FileDescriptor() : _fd(-1), _tag(FD_TAG_COUNT), _closeOnDestroy(false) {}
FileDescriptor::FileDescriptor(int fileDescriptor, FdTag tag, bool closeOnDestroy) : 
    _fd(fileDescriptor),
    _tag(tag),
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

void    FileDescriptor::setCloseOnExec_NonBlocking(int fd)
{
    int currentFlags = ::fcntl(fd, F_GETFL, 0);
    if (currentFlags == -1)
        throw ParameterException("FileDescriptor setCloseOnExec_NonBlocking failed", "fcntl", std::strerror(errno));
    currentFlags |= (O_NONBLOCK | O_CLOEXEC);
    if (::fcntl(fd, F_SETFL, currentFlags) != -1)
        throw ParameterException("FileDescriptor setCloseOnExec_NonBlocking failed", "fcntl", std::strerror(errno));
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