

#ifndef TEST_DEPENDENCIES_HPP

# define TEST_DEPENDENCIES_HPP

# include "../../Ws_Namespace.h"
# include "../../../Toolkit/ThreadPool/ThreadPool.hpp"
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../TimerTracker/Timer/Timer.hpp"
# include "../../Globals/Globals.hpp"
# include "../../Events/Manager/Manager.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../Monitor/Monitor.hpp"
# include "../Socket/Socket.hpp"
# include "AppLayerProtos.hpp"
# include <vector>

Ws::Sock::addr_in createSockAddr_in(const std::string& ip, const std::string& port);
void    prepareBindAddresses(std::vector<Ws::BindInfo>& bindAddresses, const size_t countListeners);

class Socket;
class Globals;

struct TestConnector
{
	int connect(Socket& socket);
	void disconnect();

	Ws::Sock::fd m_socket;
};

class ClientTask : public IThreadTask
{
	public:
		ClientTask(TestConnector& connector, Socket& socket);
		void execute();

	private:
		TestConnector& m_connector;
		Socket& m_socket;
};

template <typename Client>
class ClientManager;

struct ManagedClient
{
	ManagedClient();

	ClientManager<ManagedClient>* myManager();
	int open();
	int connect();
	void disconnect();
	static void CallbackSuccess(Events::Subscription& sub);
	
	unsigned char 						received;
	Socket 								m_socket;
	Monitor			 					m_monitor;
	Events::Manager* 					m_eventManager;
	ClientManager<ManagedClient>*		m_clientManager;
};

template <typename Client>
struct ClientManager
{
	ClientManager(const size_t countConnectors, const size_t countListeners, Events::Manager& eventManager)   : success(0), fail(0), m_connectors(countConnectors)
	{
		for (size_t i = 0; i < countConnectors; ++i)
		{
			m_connectors.emplace_back();
			m_connectors[i].m_socket.modifyBindInfo() = (Ws::BindInfo)
			{
				.appLayer = Ws::AppLayer::HTTP,
				.backlog = 128,
				.family = AF_INET,
				.socktype = SOCK_STREAM,
				.proto = IPPROTO_TCP,
				.addr = (Ws::Sock::union_addr){.sockaddr_in = createSockAddr_in("127.0.0.1", TestHelpers::to_string(8080 + (i % countListeners)))},
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
		ClientManagerTask(const size_t countConnectors, const size_t countListeners, Globals& globals, pthread_mutex_t& mutex, int& exitSignal)
		    : countConnectors(countConnectors), countListeners(countListeners), globals(globals), mutex(mutex), exitSignal(exitSignal) {}
		void execute()
		{
			Events::Manager clientEvents(countConnectors, globals);
			ClientManager<Client> clientManager(countConnectors, countListeners, clientEvents);
			Timer timeout = Timer::now() + 200;

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
			exitSignal = 1;
			pthread_mutex_unlock(&mutex);
		}

	private:
		size_t 				countConnectors;
		const size_t 		countListeners;
		Globals& 			globals;
		pthread_mutex_t& 	mutex;
		int& 				exitSignal;
};



#endif