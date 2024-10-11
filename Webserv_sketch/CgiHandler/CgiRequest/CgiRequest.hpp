/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 11:42:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/11 17:01:48 by manuel           ###   ########.fr       */
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

        Nginx_MemoryPool*           m_requestDataPool;
        Nginx_PoolAllocator<char>   m_strAlloc;

        const char*                 m_scriptPath;
        char**                      m_argv;
        char**                      m_envp;
        const char*                 m_stdinData;

};



#endif
