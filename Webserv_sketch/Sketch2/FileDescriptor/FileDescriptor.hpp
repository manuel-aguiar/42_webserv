/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:25:09 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 15:31:54 by mmaria-d         ###   ########.fr       */
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
        FileDescriptor(int fileDescriptor, bool markToClose = false);

        virtual ~FileDescriptor();

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
        bool                _closeOnDestruct;
        void                _setCloseOnDestruct(const bool markToClose);
        bool                _getCloseOnDestruct() const ;
        bool                _addFlags(int flags);
       
        
        //avoid duplicate fds, the new fd/the one that gets assigned to gets the responsibility
        //to close it
        FileDescriptor(const FileDescriptor& other);
        FileDescriptor& operator=(const FileDescriptor& other);

    private:
        

        //já vejo onde ponho isto

};


#endif