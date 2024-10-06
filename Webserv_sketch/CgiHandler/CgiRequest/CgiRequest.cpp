/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 11:44:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/06 14:21:33 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiRequest.hpp"

#include "../python-cgi/pythonCgi.hpp"

CgiRequest::CgiRequest() :
    _requestDataPool(Nginx_MemoryPool::create(4096)),
    _strAlloc(*_requestDataPool),
    _argv(NULL),
    _envp(NULL),
    _stdinData(NULL)
{
    
}

CgiRequest::~CgiRequest()
{
    _requestDataPool->destroy();
}

void    CgiRequest::reset()
{
    _requestDataPool->reset();
    _argv = NULL;
    _envp = NULL;
    _stdinData = NULL;
    _scriptPath = NULL;
}

void CgiRequest::initPython(PythonCgi& pythonCgi, const char* scriptPath)
{
    pythonCgi.prepareCgi(*this, scriptPath);
}

void CgiRequest::debugPrintInputs()
{
    std::cout << "argv: " << std::endl;
    for (int i = 0; _argv[i]; i++)
        std::cout << _argv[i] << std::endl;
    std::cout << "envp: " << std::endl;
    for (int i = 0; _envp[i]; i++)
        std::cout << _envp[i] << std::endl;
    std::cout << "stdinData: " << _stdinData << std::endl;
}

void   CgiRequest::execute()
{
     // Use pipe for passing data through stdin (if necessary)
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        std::cerr << "Pipe failed: " << strerror(errno) << std::endl;
        return;
    }

    // Fork process to execute the script
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Fork failed: " << strerror(errno) << std::endl;
        return;
    }

    if (pid == 0) { // Child process
        // Redirect stdin to the input side of the pipe
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]); // Close the output side of the pipe in the child

        // Execute the CGI script
        execve(_argv[0], _argv, _envp);

        // If execve fails
        std::cerr << "Exec failed: " << strerror(errno) << std::endl;
        _exit(EXIT_FAILURE);

    } else { // Parent process

        close(pipefd[0]); // Close the input side of the pipe in the parent

        // If there is data to pass to stdin (e.g., POST data), write to the pipe
        // In your case, it could be something like _RequestEnvp[PY_CONTENT_LENGTH], etc.
        const char* postData = "sample data for POST request"; // Replace with actual data
        write(pipefd[1], postData, strlen(postData)); // Write data to stdin of child
        close(pipefd[1]); // Close the pipe after writing

        // Wait for the child process to complete
        int status;
        waitpid(pid, &status, 0);
    }   
}

CgiRequest::CgiRequest(const CgiRequest &other) : 
    _requestDataPool(other._requestDataPool),
    _strAlloc(other._strAlloc),
    _argv(other._argv),
    _envp(other._envp),
    _stdinData(other._stdinData)
{
    *this = other;
}

CgiRequest &CgiRequest::operator=(const CgiRequest &other)
{
    if (this == &other)
        return (*this);
    return (*this);
}
