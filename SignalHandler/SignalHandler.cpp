/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 08:02:48 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:44:03 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "SignalHandler.hpp"
# include "../Globals/Globals.hpp"
# include "../FileDescriptor/FileDescriptor.hpp"

Globals*                                SignalHandler::gm_globals = NULL;   
int		                                SignalHandler::gm_signal = 0;
std::vector<std::pair<int, int> >       SignalHandler::gm_pipes;

int SignalHandler::PipeRead(int serverID)
{
    return (gm_pipes[serverID].first);
}

int SignalHandler::PipeWrite(int serverID)
{
    return (gm_pipes[serverID].second);
}

int SignalHandler::getSignal()
{
    return (SignalHandler::gm_signal);
};

void		SignalHandler::signal_handler(int sigNum)
{
    if (sigNum == SIGINT || sigNum == SIGQUIT)
    {
        gm_signal = sigNum;
        for (size_t i = 0; i < gm_pipes.size(); ++i)
            write(PipeWrite(i), "DUKE NUKEM", sizeof("DUKE NUKEM"));
    }
}

int SignalHandler::prepare_signal(t_sigaction *sigact, void (*handler)(int), int numServers, Globals* globals)
{
    int pipefd[2];

    assert(globals != NULL);

    gm_globals = globals;
    gm_pipes.reserve(numServers);
    
    sigact->sa_flags = SA_RESTART;
    sigact->sa_handler = handler;

    if (sigemptyset(&(sigact->sa_mask)) == -1)
    {
        gm_globals->m_logFile->record("sigemptyset(): " + std::string(std::strerror(errno)));
        throw std::runtime_error("CRITICAL: sigemptyset() failed");
    }

    if (sigaction(SIGINT, sigact, NULL) == -1)
    {
        gm_globals->m_logFile->record("sigact(): " + std::string(std::strerror(errno)));
        throw std::runtime_error("CRITICAL: sigact() failed");
    }

    if (sigaction(SIGQUIT, sigact, NULL) == -1)
    {
        gm_globals->m_logFile->record("sigact(): " + std::string(std::strerror(errno)));
        throw std::runtime_error("CRITICAL: sigact() failed");
    }

    for (int i = 0; i < numServers; ++i)
    {
        if (pipe(pipefd) == -1)
        {
            gm_globals->m_logFile->record("pipe(): " + std::string(std::strerror(errno)));
            throw std::runtime_error("CRITICAL: pipe() failed");
        }
        if (!FileDescriptor::setCloseOnExec_NonBlocking(pipefd[0])
        || !FileDescriptor::setCloseOnExec_NonBlocking(pipefd[1]))
        {
            gm_globals->m_logFile->record("fcntl(): " + std::string(std::strerror(errno)));
            throw std::runtime_error("CRITICAL: fcntl() failed");
        }
       
        gm_pipes.push_back(std::make_pair(pipefd[0], pipefd[1]));
    }
    return (1);
}

void SignalHandler::destroy_signal(t_sigaction *sigact)
{
    (void)sigact;

    for (size_t i = 0; i < gm_pipes.size(); ++i)
    {
        if (close(PipeRead(i)) == -1)
            gm_globals->m_logFile->record("close(): " + std::string(std::strerror(errno)));
        if (close(PipeWrite(i)) == -1)
            gm_globals->m_logFile->record("close(): " + std::string(std::strerror(errno)));
    }
    gm_pipes.clear();
}

