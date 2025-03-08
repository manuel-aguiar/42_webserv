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

FileDescriptor::FileDescriptor() : m_fd(-1), _tag(FD_TAG_COUNT), _closeOnDestroy(false) {}
FileDescriptor::FileDescriptor(int fileDescriptor, FdTag tag, bool closeOnDestroy) : 
    m_fd(fileDescriptor),
    _tag(tag),
    _closeOnDestroy(closeOnDestroy) {}

FileDescriptor::~FileDescriptor() {}

int FileDescriptor::getFd() const
{
    return (m_fd);
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
    int currentFlags = ::fcntl(m_fd, F_GETFL, 0);
    if (currentFlags == -1)
        return (false);    
    return (::fcntl(m_fd, F_SETFL, currentFlags | flags) != -1);
}

void    FileDescriptor::close()
{
    if (m_fd != -1)
        ::close(m_fd);
    m_fd = -1;
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

FileDescriptor::FileDescriptor(const FileDescriptor& hardCopy)  : m_fd(hardCopy.m_fd), _closeOnDestroy(hardCopy._closeOnDestroy) {(void)hardCopy;}
FileDescriptor& FileDescriptor::operator=(const FileDescriptor& hardAssign)
{
    if (this == &hardAssign)
        return (*this);
    m_fd = hardAssign.m_fd;
    _closeOnDestroy = hardAssign._closeOnDestroy;
    return (*this);
}