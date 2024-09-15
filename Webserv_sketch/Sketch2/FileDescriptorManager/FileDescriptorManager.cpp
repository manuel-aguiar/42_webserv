/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptorManager.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 10:25:22 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 12:00:44 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "FileDescriptorManager.hpp"

FileDescriptorManager::FileDescriptorManager(IEventPoll* poll) :
    _epoll(poll)
{
    if (_epoll)
        addFileDescriptor(UniquePtr<FileDescriptor>(dynamic_cast<FileDescriptor*> (_epoll)), false);
}

FileDescriptorManager::~FileDescriptorManager()
{
    for (std::map<int, FileDescriptor*>::iterator it = _openFds.begin(); it != _openFds.end(); it++)
    {
        delete it->second;
    }
    _openFds.clear();
}

void FileDescriptorManager::addFileDescriptor(UniquePtr<FileDescriptor> newFd, bool willMonitor)
{
    FileDescriptor* fd = newFd.release();
    _openFds[fd->getFd()] = fd;
    if (willMonitor && _epoll)
        _epoll->add(fd->getFd(), 1);
}

void FileDescriptorManager::removeFileDescriptor(const int fd)
{
    assert(_openFds.find(fd) != _openFds.end());

    delete (_openFds[fd]);
    _openFds.erase(fd);
}

FileDescriptor* FileDescriptorManager::getFileDescriptor(const int fd)
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



