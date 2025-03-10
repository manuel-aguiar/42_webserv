

// Project headers
# include "../../Webserv/Globals/Globals.hpp"
# include "../../Webserv/Globals/Clock/Clock.hpp"
# include "../../Webserv/Globals/LogFile/LogFile.hpp"
# include "../../Webserv/SignalHandler/SignalHandler.hpp"
# include "../../Webserv/ServerConfig/ServerConfig/ServerConfig.hpp"
# include "../../Webserv/ServerConfig/ServerBlock/ServerBlock.hpp"
# include "../../Webserv/ServerConfig/BlockFinder/BlockFinder.hpp"
# include "../../Webserv/ServerConfig/DefaultConfig/DefaultConfig.hpp"
# include "../../Webserv/ServerContext/ServerContext.hpp"
# include "../../Webserv/CgiModule/Module/Module.hpp"
# include "../../Webserv/HttpModule/HttpModule/HttpModule.hpp"
# include "../../Webserv/Events/Subscription/Subscription.hpp"
# include "../../Webserv/Events/Manager/Manager.hpp"
# include "../../Webserv/Connections/Manager/Manager.hpp"

# include "../../Toolkit/ThreadPool/ThreadPool.hpp"


// C++ headers
# include <iostream>
# include <cstdlib>
# include <cstdio> // EXIT macros

// C headers
# include <signal.h>
# include <unistd.h> // sleep

int	WebservRun(const char* programName, ServerConfig& config);
int SingleServerRun(const char* programName, ServerConfig& config, Globals& globals, const int myServerNumber);
int MultiServerRun(const char* programName, ServerConfig& config, Globals& globals, const int numServers);

int maxEventsEstimate(const ServerConfig& config)
{
	int res = 0;
	res += config.getMaxConnections();
	res += config.getAllBindAddresses().size(); // listening sockets
	res += config.getMaxConcurrentCgi() * 3;  	// read + write + emergency, 1 event each
	res += 1;		
	
	return (res);
}

int maxFdsEstimate(const ServerConfig& config)
{
	int res = 0;
	res += config.getMaxConnections() * 2;			// connections sockets + static files
	res += config.getMaxConcurrentCgi() * 6; 		// read + write + emergency, both pipe ends
	res += config.getAllBindAddresses().size(); 	// listening sockets
	res += 2;										// signal handler pipe
	res *= config.getMaxWorkers();					// workers

	return (res);
}

size_t blockFinderEntryCount(const ServerConfig& config)
{
	size_t res = 0;
	const std::vector<ServerBlock>& blocks = config.getServerBlocks();

	for (size_t i = 0; i < blocks.size(); ++i)
	{
		res += blocks[i].getListenAddresses().size() * blocks[i].getServerNames().size();
	}

	res += config.getAllBindAddresses().size(); // default block

	return (res);
}

struct RunCheck
{
	RunCheck() : yes(true) {}

	static void Callback(Events::Subscription& event)
	{
		RunCheck* check = static_cast<RunCheck*>(event.accessUser());
		check->yes = false;
	}

	bool yes;
};

int	WebservRun(const char* programName, ServerConfig& config)
{
	Clock			clock;
	LogFile			statusFile("status.log");
	LogFile			errorFile("error.log");
	LogFile			debugFile("debug.log");
	Globals			globals(&clock, &statusFile, &errorFile, &debugFile);


	int numServers = config.getMaxWorkers();

	// setup signal handling
	g_SignalHandler.openSignalListeners(numServers, &globals);
	g_SignalHandler.registerSignal(SIGINT, &globals);
	g_SignalHandler.registerSignal(SIGTERM, &globals);
	g_SignalHandler.registerSignal(SIGQUIT, &globals);
	g_SignalHandler.ignoreSignal(SIGPIPE, &globals);

	if (numServers == 1)
		return (SingleServerRun(programName, config, globals, 0));
	return (MultiServerRun(programName, config, globals, numServers));
}

int SingleServerRun(const char* programName, ServerConfig& config, Globals& globals, const int myServerNumber)
{
	////////////////////////////////////
	////////// Single Server ///////////
	////////////////////////////////////

	try
	{
		// our number 10
		Events::Manager	eventManager(maxEventsEstimate(config), globals, maxFdsEstimate(config));

		// preparing ServerContext;
		ServerContext	context;
		context.setServerConfig(config);

		BlockFinder		blockFinder(blockFinderEntryCount(config));
		context.setBlockFinder(blockFinder);

		Cgi::Module		cgiModule(config.getMaxConcurrentCgi(), config.getMaxCgiBacklog(), 5000, 4096, eventManager, globals);
		context.setAddonLayer(Ws::AddonLayer::CGI, &cgiModule);		
		
		Http::Module	httpModule(config.getMaxConnections(), context);
		context.setAppLayer(Ws::AppLayer::HTTP, &httpModule, &Http::Module::InitConnection);
		
		blockFinder.loadServerBlocks(config.getServerBlocks());

		context.setGlobals(globals);

		// preparing server launch
		Conn::Manager	connManager(config.getMaxConnections(), config.getAllBindAddresses(), eventManager, globals, context);

		// monitor signal handler
		RunCheck run;
		Events::Subscription* signalListener = eventManager.acquireSubscription();
		signalListener->setFd(g_SignalHandler.getSignalListener(myServerNumber));
		signalListener->setMonitoredEvents(Events::Monitor::READ | Events::Monitor::ERROR | Events::Monitor::HANGUP);
		signalListener->setUser(&run);
		signalListener->setCallback(&RunCheck::Callback);
		eventManager.startMonitoring(*signalListener, false);


		// oficially open the listening sockets
		if (!connManager.init())
			throw std::runtime_error("SingleServerRun: connManager.init() failed");
		//////////////////////////////////////////
		/////////// Main Server Loop /////////////
		//////////////////////////////////////////
		while (run.yes)
		{
			int cgiTimeout = cgiModule.processRequests();	
			int httpTimeout = httpModule.closeTimedOutConnections();
			eventManager.ProcessEvents(std::min(cgiTimeout, httpTimeout));
		}
		/////////////////////////////////////////
	}
	catch(const std::exception& e)
	{
		std::cerr << programName << "webserv: CRITICAL FAILURE: " << e.what() << '\n';
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

class MultiServerTask : public IThreadTask
{
	public:
		MultiServerTask(const char* programName, ServerConfig& config, Globals& globals, const int myServerNumber) : 
			m_programName(programName), 
			m_config(config), 
			m_globals(globals),
			m_myServerNumber(myServerNumber) {}
		~MultiServerTask() {}
		
		void execute()
		{
			SingleServerRun(m_programName, m_config, m_globals, m_myServerNumber);
		}
	private:
		const char*		m_programName;
		ServerConfig&	m_config;
		Globals&		m_globals;
		const int		m_myServerNumber;
};

int	MultiServerRun(const char* programName, ServerConfig& config, Globals& globals, const int numServers)
{
	sigset_t 					threadSigSet;
	DynArray<MultiServerTask> 	tasks;


	/* Disable SIGINT and SIGQUIT for the threadpool workers*/
	sigemptyset(&threadSigSet);
	sigaddset(&threadSigSet, SIGINT);
	sigaddset(&threadSigSet, SIGQUIT);
	sigaddset(&threadSigSet, SIGTERM);

	pthread_sigmask(SIG_BLOCK , &threadSigSet, NULL);		// explicitely block sigint/quit for new threads		UNPROTECTED
	ThreadPoolHeap servers(numServers, numServers);
	pthread_sigmask(SIG_UNBLOCK, &threadSigSet, NULL);

	tasks.reserve(numServers);
	for (int i = 0; i < numServers; i++)
	{
		tasks.emplace_back(programName, config, globals, i);
		servers.addTask(tasks[i], true);
	}

	while (!g_SignalHandler.getSignal())
		::usleep(1000);

	servers.destroy(true);

	return (EXIT_SUCCESS);
}
