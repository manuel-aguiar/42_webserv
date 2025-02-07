

#ifndef _TESTCLIENTMANAGER_HPP

# define _TESTCLIENTMANAGER_HPP

# include "_TestDependencies.hpp"
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../../Toolkit/ThreadPool/ThreadPool.hpp"
# include "../../TimerTracker/Timer/Timer.hpp"
# include "../../Events/Manager/Manager.hpp"

template <typename Client>
struct ClientManager
{
	ClientManager(const size_t countConnectors, const size_t countListeners, Events::Manager& eventManager, const int portStart)   : success(0), fail(0), m_connectors(countConnectors)
	{
		for (size_t i = 0; i < countConnectors; ++i)
		{
			m_connectors.emplace_back(i);
			m_connectors[i].m_socket.modifyBindInfo() = (Ws::BindInfo)
			{
				.appLayer = Ws::AppLayer::HTTP,
				.backlog = 128,
				.family = AF_INET,
				.socktype = SOCK_STREAM,
				.proto = IPPROTO_TCP,
				.addr = (Ws::Sock::union_addr){.sockaddr_in = createSockAddr_in("127.0.0.1", TestHelpers::to_string(portStart + (i % countListeners)))},
				.addrlen = sizeof(Ws::Sock::addr_in)
			};
			m_connectors[i].m_eventManager = &eventManager;
			m_connectors[i].m_clientManager = this;
			m_connectors[i].open();
		}
	}
	~ClientManager()
	{
		for (size_t i = 0; i < m_connectors.size(); ++i)
		{
			if (m_connectors[i].m_socket.getSockFd() != Ws::FD_NONE)
			{
				m_connectors[i].disconnect();
				fail++;
			}
		}
	};
	
	void ConnectAll()
	{
		for (size_t i = 0; i < m_connectors.size(); ++i)
		{
			if (m_connectors[i].connect() == -1)
			{
				//std::cerr << "failed to connect" << std::endl;
			}
		}
	}

	int success;
	int fail;
	HeapArray<Client> m_connectors;
};

template <typename Client>
class ClientManagerTask : public IThreadTask
{
	public:
		ClientManagerTask(		const size_t 		countConnectors, 
								const size_t 		countListeners, 
								const size_t 		countMaxConnections,
								Globals& 			globals, 
								pthread_mutex_t& 	mutex, 
								int& 				pasteSuccessCount, 
								int 				timeoutMs,
								const int 			portStart) : 
		countConnectors(countConnectors), 
		countListeners(countListeners), 
		countMaxConnections(countMaxConnections),
		globals(globals), 
		mutex(mutex), 
		pasteSuccessCount(pasteSuccessCount), 
		timeoutMs(timeoutMs),
		portStart(portStart) {}
		void execute()
		{
			int maxEvents = countConnectors + countListeners;
			int maxFdsEstimate = (countConnectors + countListeners + countMaxConnections) * 1.2f;
			
			Events::Manager clientEvents(maxEvents, globals, maxFdsEstimate);
			ClientManager<Client> clientManager(countConnectors, countListeners, clientEvents, portStart);
			Timer timeout = Timer::now() + timeoutMs;

			clientManager.ConnectAll();

			while (clientEvents.getMonitoringCount() > 0)
			{
				int wait = clientEvents.ProcessEvents(-1);
				(void)wait;
				//std::cerr << "\tclient manager:events received: " << wait << ", monitoring: ;" 
				//std::cerr << clientEvents.getMonitoringCount() << ", success: " 
				//std::cerr << clientManager.success << ", failure: "  << clientManager.fail <<  std::endl;
				if (Timer::now() > timeout)
					break ;
			}

			//std::cerr << "\t\t\tclient manager done, success " << clientManager.success << ", fail " << (countConnectors - clientManager.success) << std::endl;
			
			pthread_mutex_lock(&mutex);
			pasteSuccessCount = clientManager.success;
			pthread_mutex_unlock(&mutex);
		}

	private:
		const size_t		countConnectors;
		const size_t 		countListeners;
		const size_t		countMaxConnections;
		Globals& 			globals;
		pthread_mutex_t& 	mutex;
		int& 				pasteSuccessCount;
		const int			timeoutMs;
		const int			portStart;
};

#endif