/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptorManager.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 10:25:22 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 10:43:18 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "FileDescriptorManager.hpp"

FileDescriptorManager::FileDescriptorManager(IEventManager& poll) :
    _epoll(poll)
{
    addFd(UniquePtr<FileDescriptor>(static_cast<FileDescriptor*> (&_epoll)), false);
}

FileDescriptorManager::~FileDescriptorManager()
{
    for (std::map<int, FileDescriptor*>::iterator it = _openFds.begin(); it != _openFds.end(); it++)
    {
        it->second->close();
        delete (it->second);
    }
    _openFds.clear();
}

void FileDescriptorManager::addFd(FileDescriptor* newFd, bool willMonitor)
{
    _openFds[newFd->getFd()] = newFd;
    if (willMonitor)
        _epoll.addEventFd(newFd->getFd(), 1);
}

void FileDescriptorManager::removeFd(const int fd)
{
    assert(_openFds.find(fd) != _openFds.end());

    delete (_openFds[fd]);
    _openFds.erase(fd);
}

FileDescriptor* FileDescriptorManager::getFd(const int fd)
{
    assert(_openFds.find(fd) != _openFds.end());
    return (_openFds[fd]);
}

//private
FileDescriptorManager::FileDescriptorManager(const FileDescriptorManager& copy) :
    _openFds(copy._openFds)
{
    (void)copy;
}

FileDescriptorManager& FileDescriptorManager::operator=(const FileDescriptorManager& assign)
{
    if (this == &assign)
        return (*this);
    _openFds = assign._openFds;
    return (*this);
}



