/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 11:20:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 12:55:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Server.hpp"
#include "EventPoll/EventPoll.hpp"
#include "Executer/Executer.hpp"
#include "Interpreter/Interpreter.hpp"
#include "Log/LogFile.hpp"
#include "Log/LogStream.hpp"

#include <iostream>

int main(int ac, char **av)
{
    (void)ac;
    (void)av;

    try
    {
        EventPoll   poll;
        Executer    executer;
        Interpreter interpreter;
        LogFile     logFile("teste.txt");
        LogStream   logStream(std::cout);

        Server server(logFile, poll, executer, interpreter);

        std::cout << "exiting" << std::endl;
        return (0);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return (1);
    }

    return (0);
}
