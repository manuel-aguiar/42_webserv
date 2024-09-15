/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IFileDescriptorManager.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 10:24:48 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 12:16:34 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IFILEDESCRIPTORMANAGER_HPP

# define IFILEDESCRIPTORMANAGER_HPP

# include "../../../LibftCpp/libftcpp.hpp"
# include "../FileDescriptor/FileDescriptor.hpp"

class IFileDescriptorManager
{
    public:
        virtual ~IFileDescriptorManager() {}

        // FdManager is the one responsible for dealocating all filedescriptors
        virtual void                             addFileDescriptor(FileDescriptor* newFd, bool willMonitor) = 0;    
        virtual void                             removeFileDescriptor(const int fd) = 0;
        virtual FileDescriptor*                  getFileDescriptor(const int fd) = 0;

};

#endif