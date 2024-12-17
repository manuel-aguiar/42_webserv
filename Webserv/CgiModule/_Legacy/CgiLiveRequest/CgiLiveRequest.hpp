/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiLiveRequest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 11:42:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/17 12:05:34 by mmaria-d         ###   ########.fr       */
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

# include "../CgiDefaults.h"
# include "../../Webserver_Definitions.h"
# include "../../../Utils/FileDescriptor/FileDescriptor.hpp"


class CgiModule;

class CgiLiveRequest
{
    public:
        CgiLiveRequest(CgiManager& manager);
        ~CgiLiveRequest();
        CgiLiveRequest(const CgiLiveRequest &other);
        CgiLiveRequest &operator=(const CgiLiveRequest &other);

        void    reset();

        void    initPython(PythonCgi& pythonCgi, const char* scriptPath);
        void    execute();
        void    debugPrintInputs();

		void	closeAllPipes();

        Nginx_MemoryPool*           m_requestDataPool;
        Nginx_PoolAllocator<char>   m_strAlloc;

        const char*                 m_scriptPath;
        char**                      m_argPtr;
        char**                      m_envPtr;
        const char*                 m_stdinData;


		char*						m_CgiEnv[ENV_CGI_VAR_COUNT];


		CgiManager&					m_CgiModule;

	private:

		t_fd						m_ParentToChild[2];
		t_fd						m_ChildToParent[2];
		t_pid						m_pid;

};



#endif
