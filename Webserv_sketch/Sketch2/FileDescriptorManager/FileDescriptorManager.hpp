/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptorManager.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 10:25:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/17 11:13:37 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDESCRIPTORMANAGER_HPP

# define FILEDESCRIPTORMANAGER_HPP

# include "IFileDescriptorManager.hpp"
# include "../EventManager/EPollManager.hpp"
# include "../EventManager/IEventManager.hpp"
# include "../EventManager/IEventHandler.hpp"
# include <map>

class FileDescriptorManager : public IFileDescriptorManager, public IEventHandler
{
    public:
        FileDescriptorManager(IEventManager& poll);
        ~FileDescriptorManager();

        
        //inherited from IFileDescriptorManager
        void                                    addFileDescriptor(FileDescriptor* newFd, bool willMonitor);
        
        template <typename T>
        void                                    addFileDescriptor(UniquePtr<T> newFd, bool willMonitor)
        {
            FileDescriptor* fd = static_cast<FileDescriptor*>(newFd.release());
            addFileDescriptor(fd, willMonitor);
        }
        
        void                                    removeFileDescriptor(const int fd);
        FileDescriptor*                         getFileDescriptor(const int fd);

    private:
        std::map<int, FileDescriptor*>          _openFds;
        IEventManager&                          _epoll;

        FileDescriptorManager(const FileDescriptorManager& copy);
        FileDescriptorManager& operator=(const FileDescriptorManager& assign);
};


#endif