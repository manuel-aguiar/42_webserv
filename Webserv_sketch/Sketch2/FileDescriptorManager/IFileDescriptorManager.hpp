/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IFileDescriptorManager.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 10:24:48 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 10:43:18 by mmaria-d         ###   ########.fr       */
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
        virtual void                             addFd(FileDescriptor* newFd, bool willMonitor) = 0;    
        virtual void                             removeFd(const int fd) = 0;
        virtual FileDescriptor*                  getFd(const int fd) = 0;

};

#endif