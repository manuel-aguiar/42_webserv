/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 11:20:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 15:48:07 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Server.hpp"
#include "EventPoll/EventManager.hpp"
#include "Executer/Executer.hpp"
#include "Interpreter/Interpreter.hpp"
#include "Log/LogFile.hpp"
#include "Log/LogFileThreadSafe.hpp"
#include "Log/LogStream.hpp"

#include "Connection/Connection.tpp"

#include <iostream>

int RunSingleThreadedServer()
{
    EventManager    poll;
    Executer        executer;
    Interpreter     interpreter;
    LogFile         logFile("teste.txt");

    Server server(logFile, poll, executer, interpreter);

    server.run();

    return (0);
}

int RunMultiThreadedServer()
{
    EventManager            poll;
    Executer                executer;
    Interpreter             interpreter;
    LogFileThreadSafe       logFile("teste.txt");

    Server server(logFile, poll, executer, interpreter);


    server.run();

    return (0);
}

int main(int ac, char **av)
{
    (void)ac;
    (void)av;

    bool multithreaded = true;

    try
    {
        Connection<struct sockaddr_in> conn(3);

        if (multithreaded)
            RunMultiThreadedServer();
        else
            RunSingleThreadedServer();
        return (0);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return (1);
    }

    return (0);
}
