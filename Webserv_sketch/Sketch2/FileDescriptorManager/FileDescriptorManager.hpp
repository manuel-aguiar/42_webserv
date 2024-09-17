/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptorManager.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 10:25:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/17 08:20:49 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDESCRIPTORMANAGER_HPP

# define FILEDESCRIPTORMANAGER_HPP

# include "IFileDescriptorManager.hpp"
# include "../_EventPoll/EventManager.hpp"
# include <map>

class FileDescriptorManager : public IFileDescriptorManager
{
    public:
        FileDescriptorManager(IEventPoll* poll = NULL);
        ~FileDescriptorManager();

        
        //inherited from IFileDescriptorManager
        void                                    addFileDescriptor(FileDescriptor* newFd, bool willMonitor);
        
        template <typename T>
        void                                    addFileDescriptor(UniquePtr<T> newFd, bool willMonitor)
        {
            FileDescriptor* fd = dynamic_cast<FileDescriptor*>(newFd.release());
            addFileDescriptor(fd, willMonitor);
        }
        
        void                                    removeFileDescriptor(const int fd);
        FileDescriptor*                         getFileDescriptor(const int fd);

    private:
        std::map<int, FileDescriptor*>          _openFds;
        IEventPoll*                             _epoll;


        

        FileDescriptorManager(const FileDescriptorManager& copy);
        FileDescriptorManager& operator=(const FileDescriptorManager& assign);

};


#endif