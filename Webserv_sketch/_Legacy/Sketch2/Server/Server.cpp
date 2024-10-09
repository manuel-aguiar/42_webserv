/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:29:55 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 10:33:09 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(ILog& logger, IEventPoll& poll, IExecuter& executer, IInterpreter& interpreter, IFileDescriptorManager& fdManager) : 
    _logger(logger), 
    _poll(poll), 
    _executer(executer), 
    _interpreter(interpreter),
    _fdManager(fdManager) {}


void    Server::run()
{
    
}