/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 08:02:48 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/12 14:51:29 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "SignalHandler.hpp"
# include "../Globals.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

SignalHandler g_SignalHandler;

SignalHandler::SignalHandler()
{
	if (SignalHandler::m_counter != 0)
		throw std::runtime_error("SignalHandler::SignalHandler: SignalHandler is a singleton");
	SignalHandler::m_counter++;
}

SignalHandler::~SignalHandler()
{
	
}



int		SignalHandler::PipeRead(int serverID)
{
	return (m_pipes[serverID].first);
}

int		SignalHandler::PipeWrite(int serverID)
{
	return (m_pipes[serverID].second);
}

int		SignalHandler::getSignal()
{
	return (SignalHandler::m_signal);
};

void		SignalHandler::signal_handler(int sigNum)
{
	if (sigNum == SIGINT || sigNum == SIGQUIT)
	{
		m_signal = sigNum;
		for (size_t i = 0; i < m_pipes.size(); ++i)
			write(PipeWrite(i), "DUKE NUKEM", sizeof("DUKE NUKEM"));
	}
}

int		SignalHandler::prepare_signal(t_sigaction *sigact, void (*handler)(int), int numServers, Globals* globals)
{
	int pipefd[2];

	assert(globals != NULL);

	m_globals = globals;
	m_pipes.reserve(numServers);

	sigact->sa_flags = SA_RESTART;
	sigact->sa_handler = handler;

	if (sigemptyset(&(sigact->sa_mask)) == -1)
	{
		m_globals->logStatus("SignalHandler::prepare_signal, sigemptyset(): " + std::string(std::strerror(errno)));
		throw std::runtime_error("SignalHandler::prepare_signal: sigemptyset() failed");
	}

	if (sigaction(SIGINT, sigact, NULL) == -1)
	{
		m_globals->logStatus("sigact(): " + std::string(std::strerror(errno)));
		throw std::runtime_error("SignalHandler::prepare_signal: sigact() failed");
	}

	if (sigaction(SIGQUIT, sigact, NULL) == -1)
	{
		m_globals->logStatus("sigact(): " + std::string(std::strerror(errno)));
		throw std::runtime_error("SignalHandler::prepare_signal: sigact() failed");
	}

	for (int i = 0; i < numServers; ++i)
	{
		if (pipe(pipefd) == -1)
		{
			m_globals->logStatus("pipe(): " + std::string(std::strerror(errno)));
			throw std::runtime_error("SignalHandler::prepare_signal, pipe() failed");
		}
		if (!FileDescriptor::setCloseOnExec_NonBlocking(pipefd[0])
		|| !FileDescriptor::setCloseOnExec_NonBlocking(pipefd[1]))
		{
			m_globals->logStatus("fcntl(): " + std::string(std::strerror(errno)));
			throw std::runtime_error("SignalHandler::prepare_signal, fcntl() failed");
		}

		m_pipes.push_back(std::make_pair(pipefd[0], pipefd[1]));
	}
	return (1);
}

void	SignalHandler::destroy_signal(t_sigaction *sigact)
{
	(void)sigact;

	for (size_t i = 0; i < m_pipes.size(); ++i)
	{
		if (close(PipeRead(i)) == -1)
			m_globals->logStatus("close(): " + std::string(std::strerror(errno)));
		if (close(PipeWrite(i)) == -1)
			m_globals->logStatus("close(): " + std::string(std::strerror(errno)));
	}
	m_pipes.clear();
}

// private
SignalHandler::SignalHandler(const SignalHandler& copy)
{
}

SignalHandler& SignalHandler::operator=(const SignalHandler& assign)
{
	return (*this);
}