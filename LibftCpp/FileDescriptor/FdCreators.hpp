/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FdCreators.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:02:07 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 15:07:14 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef FDCREATORS_HPP

# define FDCREATORS_HPP

# include "../libftcpp.hpp"
# include "FileDescriptor.hpp"
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/epoll.h>

class FdCreators
{
    public:
        static UniquePtr<FileDescriptor> open(const char* path, int flags, int mode = 0)
        {
            return (UniquePtr<FileDescriptor>(new FileDescriptor(::open(path, flags, mode))));
        }
        static UniquePtr<FileDescriptor> socket(int domain, int type, int protocol)
        {
            return (UniquePtr<FileDescriptor>(new FileDescriptor(::socket(domain, type, protocol))));
        }
        static UniquePtr<FileDescriptor> epoll_create(int size)
        {
            return (UniquePtr<FileDescriptor>(new FileDescriptor(::epoll_create(size))));
        }
};



#endif
