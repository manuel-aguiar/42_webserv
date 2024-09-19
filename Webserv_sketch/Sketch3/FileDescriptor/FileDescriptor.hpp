/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:25:09 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 11:18:10 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDESCRIPTOR_HPP

# define FILEDESCRIPTOR_HPP

# include <fcntl.h>
# include <unistd.h>

#include <iostream>
#include <cerrno>

class FileDescriptor {

    public:
        FileDescriptor();
        FileDescriptor(int fileDescriptor, bool closeOnDestroy = true);

        ~FileDescriptor();

        int     getFd() const;
        void    setCloseOnDestroy(const bool closeOnDestroy);
        bool    getCloseOnDestroy() const;
        
        //      to access the fd
        int     operator()() const;

        // for simple comparison
        bool    operator==(const int other) const;
        bool    operator==(const FileDescriptor& other) const;

        // for STL ordering, std::lower
        bool    operator<(const int other) const;
        bool    operator<(const FileDescriptor& other) const;

        bool    setCloseOnExec_NonBlocking();

        bool    redirectTo   (const int newFd);
        bool    redirectTo   (const FileDescriptor& newFd);

        void    close(); // allow derived to override in case they have extra resources

        FileDescriptor(const FileDescriptor& hardCopy);
        FileDescriptor& operator=(const FileDescriptor& hardAssign);

    protected:
        int                 _fd;
        bool                _closeOnDestroy;
        
        //helper
        bool                _addFlags(int flags);

};


#endif