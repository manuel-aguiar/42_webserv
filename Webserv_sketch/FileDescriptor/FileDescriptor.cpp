/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 09:49:34 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 10:00:51 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "FileDescriptor.hpp"
# include "../Globals/Globals.hpp"


int  FileDescriptor::setNonBlocking(t_fd fd, Globals* globals)
{
    return (_addFlags(fd, O_NONBLOCK, globals));
}

int  FileDescriptor::setCloseOnExec(t_fd fd, Globals* globals)
{
    return (_addFlags(fd, O_CLOEXEC, globals));
}

int  FileDescriptor::setCloseOnExec_NonBlocking(t_fd fd, Globals* globals)
{
    return (_addFlags(fd, O_NONBLOCK | O_CLOEXEC, globals));
}


int    FileDescriptor::_addFlags(t_fd fd, const int flags, Globals* globals)
{
    int currentFlags = ::fcntl(fd, F_GETFL, 0);
    if (currentFlags == -1)
    {
        globals->_logFile->record("fcntl(): " + std::string(std::strerror(errno)));
        return (0);
    }
    if (::fcntl(fd, F_SETFL, currentFlags | flags) == -1)
    {
        globals->_logFile->record("fcntl(): " + std::string(std::strerror(errno)));
        return (0);
    }
    return (1);
}
