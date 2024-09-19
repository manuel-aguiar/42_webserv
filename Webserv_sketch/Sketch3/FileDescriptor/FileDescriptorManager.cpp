/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptorManager.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:03:58 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 17:00:37 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileDescriptorManager.hpp"

FileDescriptorManager::FileDescriptorManager() {}
FileDescriptorManager::~FileDescriptorManager()
{
    for (std::map<int, FileDescriptor>::iterator it = _openFds.begin(); it != _openFds.end(); it++)
    {
        if (it->second.getCloseOnDestroy())
        {
            it->second.close();
        }
    }
    _openFds.clear();
}

FileDescriptor*    FileDescriptorManager::addFd(const FileDescriptor& newFd)
{
    assert(_openFds.find(newFd.getFd()) == _openFds.end());
    _openFds[newFd.getFd()] = newFd;
    return (&_openFds[newFd.getFd()]);
}

void                FileDescriptorManager::removeFd(const int fd)
{
    assert(_openFds.find(fd) != _openFds.end());
    
    if (_openFds[fd].getCloseOnDestroy())
    {
        _openFds[fd].close();
    }
    _openFds.erase(fd);
}

FileDescriptor&    FileDescriptorManager::getFd(const int fd)
{
    assert(_openFds.find(fd) != _openFds.end());
    return (_openFds[fd]);
}

void        FileDescriptorManager::addSocket(const Socket& sock)
{
    _sockInfo[sock.getFd()] = sock;
}

FileDescriptorManager::FileDescriptorManager(const FileDescriptorManager& copy) : _openFds(copy._openFds) {}
FileDescriptorManager& FileDescriptorManager::operator=(const FileDescriptorManager& assign)
{
    if (this == &assign)
        return (*this);

    _openFds = assign._openFds;
    return (*this);
}