/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 11:15:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 08:13:27 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP

# define SERVER_HPP

# include "../FileDescriptor/FileDescriptor.hpp"
# include "../EventManager/IEventManager.hpp"
# include "../Executer/IExecuter.hpp"
# include "../Interpreter/IInterpreter.hpp"
# include "../FileDescriptorManager/IFileDescriptorManager.hpp"
# include "../Log/ILog.hpp"

# include <map>

class Server
{
    public:
        Server(ILog& logger, IEventManager& poll, IExecuter& executer, IInterpreter& interpreter, IFileDescriptorManager& fdManager);
        void run();

    private:
        const ILog&                           _logger;
        const IEventManager&                     _poll;
        const IExecuter&                      _executer;
        const IInterpreter&                   _interpreter;
        const IFileDescriptorManager&         _fdManager;

};


#endif