/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 09:49:34 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/13 10:11:31 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "FileDescriptor.hpp"
# include "../../Globals/Globals.hpp"


int  FileDescriptor::setNonBlocking(t_fd fd)
{
	return (mf_addFlags(fd, O_NONBLOCK));
}

int  FileDescriptor::setCloseOnExec(t_fd fd)
{
	return (mf_addFlags(fd, O_CLOEXEC));
}

int  FileDescriptor::setCloseOnExec_NonBlocking(t_fd fd)
{
	return (mf_addFlags(fd, O_NONBLOCK | O_CLOEXEC));
}


int    FileDescriptor::mf_addFlags(t_fd fd, const int flags)
{
	int currentFlags = ::fcntl(fd, F_GETFL, 0);
	if (currentFlags == -1)
		return (0);
	if (::fcntl(fd, F_SETFL, currentFlags | flags) == -1)
		return (0);
	return (1);
}
