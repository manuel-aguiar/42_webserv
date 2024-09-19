/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptorManager.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:02:17 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 16:25:08 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDESCRIPTORMANAGER_HPP

# define FILEDESCRIPTORMANAGER_HPP

# include <set>
# include <map>
# include <cassert>
# include "FileDescriptor.hpp"
# include "Socket.hpp"

class FileDescriptorManager
{
    public:
        FileDescriptorManager();
        ~FileDescriptorManager();

        FileDescriptor*          addFd(const FileDescriptor& newFd);
        void                     removeFd(const int fd);
        FileDescriptor&          getFd(const int fd);

    private:
        std::map<int, FileDescriptor>           _openFds;
        std::map<int, Socket>                   _sockInfo;
         
        FileDescriptorManager(const FileDescriptorManager& copy);
        FileDescriptorManager& operator=(const FileDescriptorManager& assign);
};





#endif