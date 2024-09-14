/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:25:09 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/14 11:33:37 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDESCRIPTOR_HPP

# define FILEDESCRIPTOR_HPP

# include <fcntl.h>
# include <unistd.h>

/*
    FileDescriptor class doesn't not open fds, so it is not responsible for closing them

    Virtual Destructor -> FileDescriptor is a base class that is expected to be derived from
*/

#include <iostream>
#include <cerrno>

class FileDescriptor {

    public:
        FileDescriptor();
        FileDescriptor(int fileDescriptor);

        //move
        FileDescriptor(FileDescriptor& moveCopy);
        FileDescriptor& operator=(FileDescriptor& moveAssign);

        //copy
        FileDescriptor(const FileDescriptor& hardCopy);
        FileDescriptor& operator=(const FileDescriptor& hardAssign);

        virtual ~FileDescriptor();

        int     getFd() const;

        //      to access the fd
        int     operator()() const;

        // for simple comparison
        bool    operator==(const int other) const;
        bool    operator==(const FileDescriptor& other) const;

        // for STL ordering, std::lower
        bool    operator<(const int other) const;
        bool    operator<(const FileDescriptor& other) const;

        bool    setCloseOnExec_NonBlocking();

        void    close();

        bool    redirectTo   (const int newFd);
        bool    redirectTo   (const FileDescriptor& newFd);

    protected:
        int                 _fd;



        //helper
        bool                _addFlags(int flags);

};


#endif