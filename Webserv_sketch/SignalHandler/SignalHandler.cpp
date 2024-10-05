/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 08:02:48 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/05 16:26:08 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "SignalHandler.hpp"
# include "../Globals/Globals.hpp"
# include "../FileDescriptor/FileDescriptor.hpp"

Globals*                                SignalHandler::_g_globals = NULL;   
int		                                SignalHandler::_g_signal = 0;
std::vector<std::pair<int, int> >       SignalHandler::_g_pipes;

int SignalHandler::PipeRead(int serverID)
{
    return (_g_pipes[serverID].first);
}

int SignalHandler::PipeWrite(int serverID)
{
    return (_g_pipes[serverID].second);
}

int SignalHandler::getSignal()
{
    return (SignalHandler::_g_signal);
};

void		SignalHandler::signal_handler(int sigNum)
{
    if (sigNum == SIGINT || sigNum == SIGQUIT)
    {
        _g_signal = sigNum;
        for (size_t i = 0; i < _g_pipes.size(); ++i)
            write(PipeWrite(i), "DUKE NUKEM", sizeof("DUKE NUKEM"));
    }
}

int SignalHandler::prepare_signal(t_sigaction *sigact, void (*handler)(int), int numServers, Globals* globals)
{
    int pipefd[2];

    assert(globals != NULL);

    _g_globals = globals;
    _g_pipes.reserve(numServers);
    
    sigact->sa_flags = SA_RESTART;
    sigact->sa_handler = handler;

    if (sigemptyset(&(sigact->sa_mask)) == -1)
    {
        _g_globals->_logFile->record("sigemptyset(): " + std::string(std::strerror(errno)));
        throw std::runtime_error("CRITICAL: sigemptyset() failed");
    }

    if (sigaction(SIGINT, sigact, NULL) == -1)
    {
        _g_globals->_logFile->record("sigact(): " + std::string(std::strerror(errno)));
        throw std::runtime_error("CRITICAL: sigact() failed");
    }

    if (sigaction(SIGQUIT, sigact, NULL) == -1)
    {
        _g_globals->_logFile->record("sigact(): " + std::string(std::strerror(errno)));
        throw std::runtime_error("CRITICAL: sigact() failed");
    }

    for (int i = 0; i < numServers; ++i)
    {
        if (pipe(pipefd) == -1)
        {
            _g_globals->_logFile->record("pipe(): " + std::string(std::strerror(errno)));
            throw std::runtime_error("CRITICAL: pipe() failed");
        }
        if (!FileDescriptor::setCloseOnExec_NonBlocking(pipefd[0])
        || !FileDescriptor::setCloseOnExec_NonBlocking(pipefd[1]))
        {
            _g_globals->_logFile->record("fcntl(): " + std::string(std::strerror(errno)));
            throw std::runtime_error("CRITICAL: fcntl() failed");
        }
       
        _g_pipes.push_back(std::make_pair(pipefd[0], pipefd[1]));
    }
    return (1);
}

void SignalHandler::destroy_signal(t_sigaction *sigact)
{
    (void)sigact;

    for (size_t i = 0; i < _g_pipes.size(); ++i)
    {
        if (close(PipeRead(i)) == -1)
            _g_globals->_logFile->record("close(): " + std::string(std::strerror(errno)));
        if (close(PipeWrite(i)) == -1)
            _g_globals->_logFile->record("close(): " + std::string(std::strerror(errno)));
    }
    _g_pipes.clear();
}

