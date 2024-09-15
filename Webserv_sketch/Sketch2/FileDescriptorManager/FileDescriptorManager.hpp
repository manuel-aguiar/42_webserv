/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptorManager.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 10:25:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 11:17:05 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDESCRIPTORMANAGER_HPP

# define FILEDESCRIPTORMANAGER_HPP

# include "IFileDescriptorManager.hpp"

# include <map>

class FileDescriptorManager : public IFileDescriptorManager
{
    public:
        FileDescriptorManager();
        ~FileDescriptorManager();



        void                                    addFileDescriptor(UniquePtr<FileDescriptor> newFd);
        void                                    removeFileDescriptor(const int fd);
        const FileDescriptor&                   getFileDescriptor(const int fd) const;

    private:
        std::map<int, const FileDescriptor*>          _openFds;

        FileDescriptorManager(const FileDescriptorManager& copy);
        FileDescriptorManager& operator=(const FileDescriptorManager& assign);

};


#endif