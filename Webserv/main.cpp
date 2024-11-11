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
#include "CgiHandler/python-cgi/pythonCgi.hpp"
#include "CgiHandler/CgiRequest/CgiRequest.hpp"

#define GRET_SUCKCESS EXIT_SUCCESS

int main(int ac, char **av, char **env)
{
	(void)ac; (void)av; (void)env;

	LogFile 		log("log.txt");
	LogFile 		debug("debug.txt");
	Clock   		clock;
	Globals 		globals(&clock, &log, &log, &debug);
	t_sigaction		sigaction = (t_sigaction){};


	SignalHandler::prepare_signal(&sigaction, SignalHandler::signal_handler, 1, &globals);

	Server 		server(0, &globals);

	server.createListeners(NULL, "8080", SOCK_STREAM, AF_INET, 1000);
	server.setup_mySignalHandler();

	globals.logStatus("server starting");

	server.run();

	globals.logStatus("server closing");



	PythonCgi cgi ("/usr/bin/python3");

	cgi.printVariables();
	cgi.printEnumerators();
	CgiRequest req;



	//req.debugPrintInputs();
	req.initPython(cgi, "../Websites/Website1/cgi-bin/hello.py");
	req.execute();

	req.reset();

	req.initPython(cgi, "../Websites/Website2/cgi-bin/hello.py");
	req.execute();


	return (GRET_SUCKCESS);
}
