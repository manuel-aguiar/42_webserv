/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 11:15:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 13:31:16 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP

# define SERVER_HPP


# include "../EventPoll/IEventPoll.hpp"
# include "../Executer/IExecuter.hpp"
# include "../Interpreter/IInterpreter.hpp"
# include "../Log/ILog.hpp"


class Server
{
    public:
        Server(ILog& logger, IEventPoll& poll, IExecuter& executer, IInterpreter& interpreter);
        void run();

    private:
        ILog&                   _logger;
        IEventPoll&             _poll;
        IExecuter&              _executer;
        IInterpreter&           _interpreter;
};


#endif