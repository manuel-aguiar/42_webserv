

#ifndef _CLIENTPROTOS_HPP

# define _CLIENTPROTOS_HPP


# include "../../Ws_Namespace.h"
# include "../Monitor/Monitor.hpp"
# include "../ConnInfo/ConnInfo.hpp"
# include "../../../Toolkit/ThreadPool/ThreadPool.hpp"
# include <vector>

Ws::Sock::addr_in createSockAddr_in(const std::string& ip, const std::string& port);
void    			prepareBindAddresses(std::vector<Ws::BindInfo>& bindAddresses, const size_t countListeners, const int portStart);

class ConnInfo;
class Globals;

namespace Events
{
	class Manager;
	class Subscription;
}

struct TestConnector
{
	int connect(ConnInfo& socket);
	void disconnect();

	Ws::Sock::fd m_socket;
};

class ClientTask : public IThreadTask
{
	public:
		ClientTask(TestConnector& connector, ConnInfo& socket);
		void execute();

	private:
		TestConnector& m_connector;
		ConnInfo& m_socket;
};

template <typename Client>
class ClientManager;

/*

	Client-side protocol implementation for the FastClose and NeverClose protocols

*/
struct Client_FastNeverClose
{
	Client_FastNeverClose(int index, Events::Manager& eventManager, ClientManager<Client_FastNeverClose>& clientManager);

	ClientManager<Client_FastNeverClose>& 	myManager();
	int 									open();
	int 									connect();
	void 									disconnect();
	static void 							ReadWrite_Callback(Events::Subscription& sub);
	
	int										myIndex;
	unsigned char 							request;
	unsigned char							expectedResponse;
	unsigned char 							actualResponse;
	ConnInfo 									m_socket;
	Monitor			 						m_monitor;
	ClientManager<Client_FastNeverClose>&	m_clientManager;
};

/*
	Client-side protocol implementation of the Math protocol
*/
struct Client_Math
{
	Client_Math(int index, Events::Manager& eventManager, ClientManager<Client_Math>& clientManager);

	ClientManager<Client_Math>& 		myManager();
	int 								open();
	int 								connect();
	void 								disconnect();
	static void 						ReadWrite_Callback(Events::Subscription& sub);
	void								Read();
	void								Write();

	int									myIndex;
	unsigned char 						request;
	unsigned char						expectedResponse;
	unsigned char 						actualResponse;
	bool								hasWritten;
	ConnInfo 								m_socket;
	Monitor			 					m_monitor;
	ClientManager<Client_Math>&			m_clientManager;
};


#endif