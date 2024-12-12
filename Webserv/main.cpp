/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:12:37 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/12 10:54:00 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// Project headers
# include "Globals/Globals.hpp"
# include "ServerConfig/ServerConfig/ServerConfig.hpp"
# include "ServerManager/ServerManager.hpp"

// C++ headers
# include <iostream>

/*
	WIP

	Main function for the project
	Estabablish pseudo-global variables
	Load and parse the configuration file
	Instantiate the ServerManager

	RUN
*/
int main(int ac, char** av)
{
    if (ac != 2)
	{
		std::cerr << "Usage: " << av[0] << " <config_file>" << std::endl;
		return (1);
	}

	try
	{
		//setup
		Clock			clock;
		LogFile			statusFile("status.log");
		LogFile			errorFile("error.log");
		LogFile			debugFile("debug.log");
		Globals			globals(&clock, &statusFile, &errorFile, &debugFile);
		ServerConfig	config(av[1], &globals);
		ServerManager	webserver(config, &globals);

		//RUN
		webserver.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << av[0] << ": CRITICAL FAILURE " << e.what() << '\n';
	}
	
    return (0);
}
