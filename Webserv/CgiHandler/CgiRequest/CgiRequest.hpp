/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 11:42:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/12 10:30:00 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIREQUEST_HPP

# define CGIREQUEST_HPP

# include <cstddef>

# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>

#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <cerrno>

# include "../../Webserver_Definitions.h"
# include "../FileDescriptor/FileDescriptor.hpp"


class PythonCgi;

class CgiRequest
{
    public:
        CgiRequest();
        ~CgiRequest();
        CgiRequest(const CgiRequest &other);
        CgiRequest &operator=(const CgiRequest &other);

        void    reset();

        void    initPython(PythonCgi& pythonCgi, const char* scriptPath);
        void    execute();
        void    debugPrintInputs();

		void	closeAllPipes();

        Nginx_MemoryPool*           m_requestDataPool;
        Nginx_PoolAllocator<char>   m_strAlloc;

        const char*                 m_scriptPath;
        char**                      m_argv;
        char**                      m_envp;
        const char*                 m_stdinData;

		t_fd						m_ParentToChild[2];
		t_fd						m_ChildToParent[2];
		t_pid						m_pid;

};



#endif
