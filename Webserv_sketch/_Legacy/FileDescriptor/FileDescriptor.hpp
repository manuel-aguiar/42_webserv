/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:25:09 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:10:56 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDESCRIPTOR_HPP

# define FILEDESCRIPTOR_HPP

# include <fcntl.h>
# include <unistd.h>

#include <iostream>
#include <cerrno>

#include "ParameterException.hpp"

class FileDescriptor {

    public:

        typedef enum
        {
            FD_SOCKET_LISTENING = 0,
            FD_SOCKET_CONNECTION,
            FD_SOCKET_CLIENT,
            FD_STANDARD,
            FD_EPOLL,
            FD_PIPE_READ,
            FD_PIPE_WRITE,
            FD_TAG_COUNT
        }   FdTag;


        FileDescriptor();
        FileDescriptor(int fileDescriptor, FdTag tag, bool closeOnDestroy = true);
        ~FileDescriptor();

        int     getFd() const;
        int     getTag() const;
        void    setCloseOnDestroy(const bool closeOnDestroy);
        bool    getCloseOnDestroy() const;

        void    close();

        FileDescriptor(const FileDescriptor& hardCopy);
        FileDescriptor& operator=(const FileDescriptor& hardAssign);

        static void setCloseOnExec_NonBlocking(int fd);

    protected:
        int                 m_fd;
        FdTag               _tag;
        bool                _closeOnDestroy;
        
        bool                _addFlags(int flags);

};


#endif