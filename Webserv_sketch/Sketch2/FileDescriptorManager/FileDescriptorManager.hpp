/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptorManager.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 10:25:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 11:54:33 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDESCRIPTORMANAGER_HPP

# define FILEDESCRIPTORMANAGER_HPP

# include "IFileDescriptorManager.hpp"
# include "../EventPoll/EventManager.hpp"
# include <map>

class FileDescriptorManager : public IFileDescriptorManager
{
    public:
        FileDescriptorManager(IEventPoll* poll = NULL);
        ~FileDescriptorManager();

        void                                    addFileDescriptor(UniquePtr<FileDescriptor> newFd, bool willMonitor);
        
        void                                    removeFileDescriptor(const int fd);
        FileDescriptor*                         getFileDescriptor(const int fd);

    private:
        std::map<int, FileDescriptor*>          _openFds;
        IEventPoll*                             _epoll;

        FileDescriptorManager(const FileDescriptorManager& copy);
        FileDescriptorManager& operator=(const FileDescriptorManager& assign);

};


#endif