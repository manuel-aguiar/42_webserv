/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiLiveRequestExecute.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:25:41 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/16 09:57:32 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Headers
# include "CgiLiveRequest.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

// C Headers
# include <unistd.h>
# include <sys/wait.h>

void   CgiLiveRequest::execute()
{
	assert(m_eventManager != NULL && m_pendingRequest != NULL);

    if (::pipe(m_ParentToChild) == -1)
	{
		//write to log
        std::cerr << "Pipe failed: " << strerror(errno) << std::endl;
		//closeAllPipes();
        return;
    }
	if (::pipe(m_ChildToParent) == -1)
	{
		//write to log
		std::cerr << "Pipe failed: " << strerror(errno) << std::endl;
		//closeAllPipes();
		return;
	}
	if (!FileDescriptor::setNonBlocking(m_ParentToChild[0]) ||
		!FileDescriptor::setNonBlocking(m_ParentToChild[1]) ||
		!FileDescriptor::setNonBlocking(m_ChildToParent[0]) ||
		!FileDescriptor::setNonBlocking(m_ChildToParent[1]))
	{
		//write to log, fcntl failed, not really critical
	}

    m_pid = ::fork();
    if (m_pid == -1)
	{
		//write to log
        std::cerr << "Fork failed: " << strerror(errno) << std::endl;
		//closeAllPipes();
        return;
    }

    if (m_pid == 0)
	{

        if (::dup2(m_ParentToChild[0], STDIN_FILENO) == -1)
		{
			//write to log
			std::cerr << "Dup2 failed: " << strerror(errno) << std::endl;
			//closeAllPipes();
        	::exit(EXIT_FAILURE);
		}
        if (::close(m_ParentToChild[1]) == -1)
		{
			//write to log, non critical
		}

        //::execve(m_argv[0], m_argv, m_envp);

        // write log, exec failed
        std::cerr << "Exec failed: " << strerror(errno) << std::endl;
        ::exit(EXIT_FAILURE);

    }
	else
	{

        if (::close(m_ParentToChild[0]) == -1)
		{
			// write to log, non critical
		}

        const char* postData = "sample data for POST request";
        ::write(m_ParentToChild[1], postData, strlen(postData));
        ::close(m_ParentToChild[1]);

		/**************************/
		// subscribe pipe read to epoll
		/***************************/

        int status;
        ::waitpid(m_pid, &status, 0);
    }
}