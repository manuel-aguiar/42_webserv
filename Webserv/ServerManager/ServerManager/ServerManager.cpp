

# include "ServerManager.hpp"
# include "../../ServerConfig/ServerConfig/ServerConfig.hpp"
# include "../../Ws_Namespace.h"
# include "../../HttpModule/HttpModule.hpp"
# include "../../CgiModule/CgiModule/ImplModule.hpp"
# include "../../Globals/SignalHandler/SignalHandler.hpp"

// Toolkit headers
# include "../../../Toolkit/ThreadPool/ThreadPool.h"

ServerManager::ServerManager(const ServerConfig& config, Globals& globals) :
	m_workers(config.getNumWorkers()),
	m_blockFinder(config),
	m_config(config),
	m_globals(globals),
	m_threadPool(NULL)
{
	m_protoModules[HTTP_MODULE] = new HttpModule(*this);
	m_initProtoConnection[HTTP_MODULE] = &HttpModule::initConnection;
	mf_prepareWorkers();

	//prepare signal handler
	g_SignalHandler.prepare_signal(SignalHandler::signal_handler, m_config.getNumWorkers(), globals);
}

/*
	ServerWorkers are built inside the memory pool which doesn't call destructors
	We must call the destructors ourselves
*/
ServerManager::~ServerManager()
{
	for (size_t i = 0; i < MODULE_COUNT; ++i)
		delete ((unsigned char *)m_protoModules[i]);
	for (size_t i = 0; i < m_workers.size(); ++i)
		m_workers[i]->~ServerWorker();
	if (m_threadPool)
		delete (m_threadPool);
}

void    ServerManager::run()
{
	if (m_workers.size() == 1)
		mf_runSingleThreaded();
	else
		mf_runMultiThreaded();
}


/*
	Run ServerManager single threaded (only 1 worker to work with)
	
	Just run the single worker
*/
void    ServerManager::mf_runSingleThreaded()
{
	m_workers[0]->run();
}

/*
	Run ServerManager in a multi-threaded environment (1 worker per thread)

	Sets up signal handling such that only the main thread receives process group signals
	Opens the Threadpool that creates the threads with this signal mask
	Reinstate the previous mask

	Run.
*/


/* Lazy but it really will only be used here */
class ServerThreadTask : public IThreadTask
{
	public:
		ServerThreadTask(ServerWorker& worker) : m_worker(worker) {}
		ServerThreadTask(const ServerThreadTask& copy) : m_worker(copy.m_worker) {}
		~ServerThreadTask() {}

		void execute() { m_worker.run(); }
		
	private:
		ServerWorker& m_worker;

		ServerThreadTask& operator=(const ServerThreadTask& assign) { (void)assign; return (*this); }
};

void    ServerManager::mf_runMultiThreaded()
{
	sigset_t					threadSigSet;
	DynArray<ServerThreadTask>	tasks;

	if (::sigemptyset(&threadSigSet) != 0)
		throw std::runtime_error("ServerManager::mf_runMultiThreaded: sigemptyset failed");
	if (::sigaddset(&threadSigSet, SIGINT))
		throw std::runtime_error("ServerManager::mf_runMultiThreaded: sigaddset failed");
	if (::sigaddset(&threadSigSet, SIGQUIT))
		throw std::runtime_error("ServerManager::mf_runMultiThreaded: sigaddset failed");
	if (::pthread_sigmask(SIG_BLOCK , &threadSigSet, NULL))
		throw std::runtime_error("ServerManager::mf_runMultiThreaded: pthread_sigmask failed");

	m_threadPool = new ThreadPool(m_workers.size(), 100);										// à pata

	if (::pthread_sigmask(SIG_UNBLOCK, &threadSigSet, NULL))
		throw std::runtime_error("ServerManager::mf_runMultiThreaded: pthread_sigmask failed");

	tasks.reserve(m_workers.size());

	for (size_t i = 0; i < m_workers.size(); ++i)
	{
		tasks.emplace_back(*m_workers[i]);
		m_threadPool->addTask(tasks[i]);
	}

	while (!g_SignalHandler.getSignal())
		::usleep(1000);

	m_threadPool->destroy(true);
	delete (m_threadPool);
	m_threadPool = NULL;
}
