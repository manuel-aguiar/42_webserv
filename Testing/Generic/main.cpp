

#include "../../Webserv/Webserver_Definitions.h"
#include "../../Webserv/ServerManager/ServerManager.hpp"
#include "../../Webserv/Globals/Clock/Clock.hpp"
#include "../../Webserv/Globals/Globals.hpp"
#include "../../Webserv/Globals/SignalHandler/SignalHandler.hpp"
#include "../../Webserv/ServerManager/CgiManager/python-cgi/pythonCgi.hpp"
#include "../../Webserv/ServerManager/CgiManager/CgiLiveRequest/CgiLiveRequest.hpp"

#define GRET_SUCKCESS EXIT_SUCCESS

int main(int ac, char **av, char **env)
{
	(void)ac; (void)av; (void)env;

	LogFile 		log("../log.txt");
	LogFile 		debug("../debug.txt");
	Clock   		clock;
	Globals 		globals(&clock, &log, &log, &debug);
	t_sigaction		sigaction = (t_sigaction){};


	SignalHandler::prepare_signal(&sigaction, SignalHandler::signal_handler, 1, &globals);

	ServerManager 		server(0, &globals);

	server.createListeners(NULL, "8080", SOCK_STREAM, AF_INET, 1000);
	server.setup_mySignalHandler();

	globals.logStatus("server starting");

	server.run();

	globals.logStatus("server closing");



	PythonCgi cgi ("/usr/bin/python3");

	cgi.printVariables();
	cgi.printEnumerators();
	CgiManager manager;
	CgiLiveRequest req(manager);



	//req.debugPrintInputs();
	req.initPython(cgi, "../Websites/Website1/cgi-bin/hello.py");
	req.execute();

	req.reset();

	req.initPython(cgi, "../Websites/Website2/cgi-bin/hello.py");
	req.execute();


	return (GRET_SUCKCESS);
}
