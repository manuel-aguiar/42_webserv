

#ifndef TEST_DEPENDENCIES_HPP

# define TEST_DEPENDENCIES_HPP

# include "../../Ws_Namespace.h"
# include "../../../Toolkit/ThreadPool/ThreadPool.hpp"
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../Monitor/Monitor.hpp"
# include "../Socket/Socket.hpp"
# include <vector>


class Socket;
class Globals;

class TestConnector
{
	public:
		int connect(Socket& socket);
		void disconnect();

	private:
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


struct ManagedClient
{
	int open();
	int connect();
	void disconnect();
	static void CallbackSuccess(Events::Subscription& sub);

	Socket m_socket;
	Monitor m_monitor;
	Events::Manager* m_eventManager;
};

class ClientManager
{
	public:
		ClientManager(const size_t countConnectors, const size_t countListeners, Events::Manager& eventManager);
		~ClientManager();
		
		void ConnectAll();

	private:
		HeapArray<ManagedClient> m_connectors;
};

class ClientManagerTask : public IThreadTask
{
	public:
		ClientManagerTask(const size_t countConnectors, const size_t countListeners, Globals& globals);
		void execute();

	private:
		size_t countConnectors;
		const size_t countListeners;
		Globals& globals;
};

Ws::Sock::addr_in createSockAddr_in(const std::string& ip, const std::string& port);
void    prepareBindAddresses(std::vector<Ws::BindInfo>& bindAddresses, const size_t countListeners);

#endif