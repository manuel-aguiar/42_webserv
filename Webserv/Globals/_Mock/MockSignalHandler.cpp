/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MockSignalHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 08:02:48 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/13 09:06:44 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../SignalHandler/SignalHandler.hpp"
# include "../Globals.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

size_t			SignalHandler::gm_counter;

SignalHandler::SignalHandler()
{
	
}

SignalHandler::~SignalHandler()
{

}

t_fd		SignalHandler::getPipeRead(int serverID)
{
    (void)serverID;
	return (1);
}

t_fd		SignalHandler::getPipeWrite(int serverID)
{
    (void)serverID;
	return (1);
}

int		SignalHandler::getSignal()
{
	return (1);
};

void	SignalHandler::setSignal(int sig)
{
    (void)sig;
}

void		SignalHandler::signal_handler(int sigNum)
{
    (void)sigNum;
}

const std::vector<std::pair<int, int> >&		SignalHandler::getPipes()
{
	return (m_pipes);
}

void		SignalHandler::prepare_signal(void (*handler)(int), size_t numServers, Globals& globals)
{
    (void)handler;
    (void)numServers;
    (void)globals;
}

// private
SignalHandler::SignalHandler(const SignalHandler& copy)
{
	(void)copy;
}

SignalHandler& SignalHandler::operator=(const SignalHandler& assign)
{
	(void)assign;
	return (*this);
}