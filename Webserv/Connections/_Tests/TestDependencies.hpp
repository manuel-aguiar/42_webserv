

#ifndef TEST_DEPENDENCIES_HPP

# define TEST_DEPENDENCIES_HPP

# include "../../Ws_Namespace.h"
# include "../../../Toolkit/ThreadPool/ThreadPool.hpp"
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../Monitor/Monitor.hpp"
# include "../Socket/Socket.hpp"
# include "AppLayerProtos.hpp"
# include <vector>


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

class ClientManager;

struct ManagedClient
{
	ManagedClient();

	ClientManager* myManager();
	int open();
	int connect();
	void disconnect();
	static void CallbackSuccess(Events::Subscription& sub);
	
	unsigned char 		received;
	Socket 				m_socket;
	Monitor			 	m_monitor;
	Events::Manager* 	m_eventManager;
	ClientManager*		m_clientManager;
};

struct ClientManager
{
	ClientManager(const size_t countConnectors, const size_t countListeners, Events::Manager& eventManager);
	~ClientManager();
	
	void ConnectAll();

	int success;
	int fail;
	HeapArray<ManagedClient> m_connectors;
};

class ClientManagerTask : public IThreadTask
{
	public:
		ClientManagerTask(const size_t countConnectors, const size_t countListeners, Globals& globals, pthread_mutex_t& mutex, int& exitSignal);
		void execute();

	private:
		size_t 				countConnectors;
		const size_t 		countListeners;
		Globals& 			globals;
		pthread_mutex_t& 	mutex;
		int& 				exitSignal;
};

Ws::Sock::addr_in createSockAddr_in(const std::string& ip, const std::string& port);
void    prepareBindAddresses(std::vector<Ws::BindInfo>& bindAddresses, const size_t countListeners);

#endif