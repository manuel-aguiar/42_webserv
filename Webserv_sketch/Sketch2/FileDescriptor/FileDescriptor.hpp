/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:25:09 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 18:06:05 by mmaria-d         ###   ########.fr       */
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


class FileDescriptor {

    public:
        FileDescriptor();
        FileDescriptor(int fileDescriptor);
        FileDescriptor(const FileDescriptor& other);
        FileDescriptor& operator=(const FileDescriptor& other);
        virtual ~FileDescriptor();

        // for simple comparison
        bool    operator==(const int other) const;
        bool    operator==(const FileDescriptor& other) const;

        // for STL ordering, std::lower
        bool    operator<(const int other) const;
        bool    operator<(const FileDescriptor& other) const;

        bool    setCloseOnExec_NonBlocking();

        bool    redirectTo   (const int newFd);
        bool    redirectTo   (const FileDescriptor& newFd);

        void    setFd(const int fd);
        int     getFd() const;

    protected:
        bool    _addFlags(int flags);
        int     _fd;

};


#endif