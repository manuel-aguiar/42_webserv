/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:29:55 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 13:30:57 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(ILog& logger, IEventPoll& poll, IExecuter& executer, IInterpreter& interpreter)
        : _logger(logger), _poll(poll), _executer(executer), _interpreter(interpreter) {}


void    Server::run()
{
    _logger.record("Server started");
    _poll.poll();
    _executer.execute();
    _interpreter.interpret();
    _logger.record("Server stopped");
}