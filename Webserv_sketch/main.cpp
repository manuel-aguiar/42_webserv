/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   main.cpp										   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/26 08:43:05 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/29 14:17:08 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "Webserver_Definitions.h"
#include "Server/Server.hpp"
#include "Clock/Clock.hpp"
#include "Globals/Globals.hpp"
#include "SignalHandler/SignalHandler.hpp"

#define GRET_SUCKCESS EXIT_SUCCESS

int main(int ac, char **av, char **env)
{
	(void)ac; (void)av; (void)env;

	LogFile 	log("log.txt");
	LogFile 	debug("debug.txt");
	Clock   	clock;
	Globals 	globals(&clock, &log, &debug);
	t_sigaction	sigaction = (t_sigaction){};

	SignalHandler::prepare_signal(&sigaction, SignalHandler::signal_handler, 1, &globals);

	Server 		server(0, &globals);

	server.createListeners(NULL, "8081", SOCK_STREAM, AF_UNSPEC, 10);
	server.setup_mySignalHandler();
	
	globals._logFile->record("server starting");

	server.run();

	globals._logFile->record("server closing");

	return (GRET_SUCKCESS);
}