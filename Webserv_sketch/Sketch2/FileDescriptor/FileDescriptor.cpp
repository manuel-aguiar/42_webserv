/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:25:09 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 08:40:44 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "FileDescriptor.hpp"

FileDescriptor::FileDescriptor() : _fd(-1) {}
FileDescriptor::FileDescriptor(int fileDescriptor) : _fd(fileDescriptor) {}
FileDescriptor::FileDescriptor(const FileDescriptor& other) : _fd(other._fd) {}
FileDescriptor& FileDescriptor::operator=(const FileDescriptor& other)
{
    if (this != &other)
    {
        _fd = other._fd;
    }
    return *this;
}

bool FileDescriptor::operator==(int other) const
{
    return (_fd == other);
}

bool FileDescriptor::operator==(const FileDescriptor& other) const
{
    return (_fd == other._fd);
}

bool FileDescriptor::operator<(int other) const
{
    return (_fd < other);
}

bool FileDescriptor::operator<(const FileDescriptor& other) const
{
    return (_fd < other._fd);
}



