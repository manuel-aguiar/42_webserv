/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDescriptor.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 09:49:44 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/13 10:11:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDESCRIPTOR_HPP

# define FILEDESCRIPTOR_HPP

# include <fcntl.h>
# include "../Webserver_Definitions.h"

class Globals;

class FileDescriptor
{
	public:
		static int  setNonBlocking					(t_fd fd);
		static int  setCloseOnExec					(t_fd fd);
		static int  setFdFlags						(t_fd fd, const int flags);
		static int  setCloseOnExec_NonBlocking		(t_fd fd);
	private:
		static int mf_addFlags						(t_fd fd, const int flags);
};

#endif