/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 09:49:44 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 09:53:55 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDESCRIPTOR_HPP

# define FILEDESCRIPTOR_HPP

# include <fcntl.h>

class Globals;

class FileDescriptor
{
    static int  setNonBlocking(int fd, Globals* globals);
    static int  setCloseOnExec(int fd, Globals* globals);
    static int  setFdFlags(int fd, int flags, Globals* globals);
    static int  setCloseOnExec_NonBlocking(int fd, Globals* globals);
};

#endif