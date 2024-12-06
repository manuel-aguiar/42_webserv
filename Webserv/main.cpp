/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:12:37 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/06 11:12:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// Project headers
# include "Globals/Globals.hpp"
# include "ServerConfig/ServerConfig.hpp"
# include "ServerConfig/DefaultConfig/DefaultConfig.hpp"
# include "ServerManager/ServerManager.hpp"

/*
	WIP

	Main function for the project
	Estabablish pseudo-global variables
	Load and parse the configuration file
	Instantiate the ServerManager

	RUN
*/
int main(int ac, char** av, char** env)
{
    if (ac != 2)
	{
		std::cerr << "Usage: " << av[0] << " <config_file>" << std::endl;
		return (1);
	}

	Clock			clock;
	LogFile			statusFile("status.log");
	LogFile			errorFile("error.log");
	LogFile			debugFile("debug.log");
	Globals			globals(&clock, &statusFile, &errorFile, &debugFile);
	ServerConfig	config(av[1], DefaultConfig(), &globals);
	ServerManager	server(config, &globals);

	server.run();

    return (0);
}
