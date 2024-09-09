/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 11:15:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 11:18:42 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP

# define SERVER_HPP


# include "../EventPoll/IEventPoll.hpp"
# include "../Executer/IExecuter.hpp"
# include "../Interpreter/IInterpreter.hpp"
# include "../Log/ILog.hpp"


class Server {
public:
    Server(ILog& logger, IEventPoll& poll, IExecuter& executer, IInterpreter& interpreter)
        : _logger(logger), _poll(poll), _executer(executer), _interpreter(interpreter) {}

    void run() {
        _logger.record("Server started");
        _poll.poll();
        _executer.execute();
        _interpreter.interpret();
        _logger.record("Server stopped");
    }

private:
    ILog&                   _logger;
    IEventPoll&             _poll;
    IExecuter&              _executer;
    IInterpreter&           _interpreter;
};


#endif