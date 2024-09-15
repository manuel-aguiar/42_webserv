/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptorManager.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 10:25:22 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 10:55:11 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "FileDescriptorManager.hpp"

FileDescriptorManager::FileDescriptorManager()
{
    for (std::map<int, const FileDescriptor*>::iterator it = _openFds.begin(); it != _openFds.end(); it++)
    {
        delete it->second;
    }
    _openFds.clear();
}

FileDescriptorManager::~FileDescriptorManager()
{
    for (std::map<int, const FileDescriptor*>::iterator it = _openFds.begin(); it != _openFds.end(); it++)
    {
        delete it->second;
    }
}

void FileDescriptorManager::addFileDescriptor(const FileDescriptor* newFd)
{
    _openFds[newFd->getFd()] = newFd;
}

void FileDescriptorManager::removeFileDescriptor(const int fd)
{
    assert(_openFds.find(fd) != _openFds.end());

    delete (_openFds[fd]);
    _openFds.erase(fd);
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