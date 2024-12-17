/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiLiveRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 11:44:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/12 10:41:04 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiLiveRequest.hpp"

#include "../python-cgi/pythonCgi.hpp"

CgiLiveRequest::CgiLiveRequest(CgiManager& manager) :
    m_requestDataPool(Nginx_MemoryPool::create(4096)),
    m_strAlloc(m_requestDataPool),
    m_argPtr(NULL),
    m_envPtr(NULL),
    m_stdinData(NULL),
	m_CgiModule(manager)
{
	m_ParentToChild[0] = -1;
	m_ParentToChild[1] = -1;
	m_ChildToParent[0] = -1;
	m_ChildToParent[1] = -1;
}

CgiLiveRequest::~CgiLiveRequest()
{
    m_requestDataPool->destroy();
}

void    CgiLiveRequest::reset()
{
    m_requestDataPool->reset();
    m_argPtr = NULL;
    m_envPtr = NULL;
    m_stdinData = NULL;
    m_scriptPath = NULL;
}

void CgiLiveRequest::initPython(PythonCgi& pythonCgi, const char* scriptPath)
{
    pythonCgi.prepareCgi(*this, scriptPath);
}

void CgiLiveRequest::debugPrintInputs()
{
    std::cout << "argv: " << std::endl;
    for (int i = 0; m_argPtr[i]; i++)
        std::cout << m_argPtr[i] << std::endl;
    std::cout << "envp: " << std::endl;
    for (int i = 0; m_envPtr[i]; i++)
        std::cout << m_envPtr[i] << std::endl;
    std::cout << "stdinData: " << m_stdinData << std::endl;
}

void   CgiLiveRequest::execute()
{
    if (pipe(m_ParentToChild) == -1)
	{
		//write to log
        std::cerr << "Pipe failed: " << strerror(errno) << std::endl;
		closeAllPipes();
        return;
    }
	if (pipe(m_ChildToParent) == -1)
	{
		//write to log
		std::cerr << "Pipe failed: " << strerror(errno) << std::endl;
		closeAllPipes();
		return;
	}
	if (!FileDescriptor::setNonBlocking(m_ParentToChild[0]) ||
		!FileDescriptor::setNonBlocking(m_ParentToChild[1]) ||
		!FileDescriptor::setNonBlocking(m_ChildToParent[0]) ||
		!FileDescriptor::setNonBlocking(m_ChildToParent[1]))
	{
		//write to log, fcntl failed, not really critical
	}

    m_pid = fork();
    if (m_pid == -1)
	{
		//write to log
        std::cerr << "Fork failed: " << strerror(errno) << std::endl;
		closeAllPipes();
        return;
    }

    if (m_pid == 0)
	{

        if (dup2(m_ParentToChild[0], STDIN_FILENO) == -1)
		{
			//write to log
			std::cerr << "Dup2 failed: " << strerror(errno) << std::endl;
			closeAllPipes();
        	exit(EXIT_FAILURE);
		}
        if (close(m_ParentToChild[1]) == -1)
		{
			//write to log, non critical
		}

        execve(m_argPtr[0], m_argPtr, m_envPtr);

        // write log, exec failed
        std::cerr << "Exec failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);

    }
	else
	{

        if (close(m_ParentToChild[0]) == -1)
		{
			// write to log, non critical
		}

        const char* postData = "sample data for POST request";
        write(m_ParentToChild[1], postData, strlen(postData));
        close(m_ParentToChild[1]);

		/**************************/
		// subscribe pipe read to epoll
		/***************************/

        int status;
        waitpid(m_pid, &status, 0);
    }
}

void CgiLiveRequest::closeAllPipes()
{
	//if (m_ParentToChild[0] != -1 && close(m_ParentToChild[0]) == -1)
	//	write log
	//if (m_ParentToChild[1] != -1 && close(m_ParentToChild[1]) == -1)
	//	write log
	//if (m_ChildToParent[0] != -1 && close(m_ChildToParent[0]) == -1)
	//	write log
	//if (m_ChildToParent[1] != -1 && close(m_ChildToParent[1]) == -1)
	//	write log
}

CgiLiveRequest::CgiLiveRequest(const CgiLiveRequest &other) :
    m_requestDataPool(other.m_requestDataPool),
    m_strAlloc(other.m_strAlloc),
    m_argPtr(other.m_argPtr),
    m_envPtr(other.m_envPtr),
    m_stdinData(other.m_stdinData),
	m_CgiModule(other.m_CgiModule)
{
    *this = other;
}

CgiLiveRequest &CgiLiveRequest::operator=(const CgiLiveRequest &other)
{
    if (this == &other)
        return (*this);
    return (*this);
}
