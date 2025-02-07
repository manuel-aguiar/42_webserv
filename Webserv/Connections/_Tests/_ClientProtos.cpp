// Project headers

# include "_TestDependencies.hpp"
# include "_TestClientManager.tpp"
# include "_ClientProtos.hpp"
# include "../Socket/Socket.hpp"
# include "../../Events/Manager/Manager.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

// C headers
#include <unistd.h>     // For close()
#include <sys/socket.h> // For socket(), connect()
#include <arpa/inet.h>  // For inet_pton()

//C++ headers
#include <vector>
#include <cerrno>

int TestConnector::connect(ConnInfo& socket)
{
    m_socket = ::socket(socket.getBindInfo().family, socket.getBindInfo().socktype, socket.getBindInfo().proto);
    if (m_socket == -1)
        return -1;

    // TestConnector will block until it is accepted
    return (::connect(m_socket, (struct sockaddr*)&socket.getBindInfo().addr, socket.getBindInfo().addrlen));
}

void TestConnector::disconnect()
{
    ::close(m_socket);
}

ClientTask::ClientTask(TestConnector& connector, ConnInfo& socket)
    : m_connector(connector), m_socket(socket) {}

void ClientTask::execute()
{
    m_connector.connect(m_socket);
}


Client_FastNeverClose::Client_FastNeverClose(int index, Events::Manager& eventManager, ClientManager<Client_FastNeverClose>& clientManager) : 
    myIndex(index), 
    actualResponse(0),
    m_monitor(eventManager),
    m_clientManager(clientManager) {}

int Client_FastNeverClose::open()
{
    int sockfd = ::socket(m_socket.getBindInfo().family, m_socket.getBindInfo().socktype, m_socket.getBindInfo().proto);

    if (sockfd == -1)
        return -1;

    m_socket.setSockFd(sockfd);
    FileDescriptor::setNonBlocking(sockfd);

    m_monitor.acquire();
    Events::Subscription& sub = m_monitor.accessEvent();
    sub.setFd(m_socket.getSockFd());
    sub.setUser(this);
    sub.setMonitoredEvents(Events::Monitor::READ | Events::Monitor::ERROR | Events::Monitor::HANGUP);
    sub.setCallback(Client_FastNeverClose::ReadWrite_Callback);

    return 1;
}

int Client_FastNeverClose::connect()
{
    int res = ::connect(m_socket.getSockFd(), (struct sockaddr*)&m_socket.getBindInfo().addr, m_socket.getBindInfo().addrlen);

    if (res == -1 && errno != EINPROGRESS)
    {
        disconnect();
        m_clientManager.fail++;
    }
    m_monitor.subscribe(false);
    return 1;
}

ClientManager<Client_FastNeverClose>& Client_FastNeverClose::myManager()
{
	return m_clientManager;
}

void Client_FastNeverClose::ReadWrite_Callback(Events::Subscription& sub)
{
    Client_FastNeverClose* conn = reinterpret_cast<Client_FastNeverClose*>(sub.accessUser());
	int triggeredEvents = sub.getTriggeredEvents();
	

	if (triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP) && conn->actualResponse == 0)
	{
		conn->disconnect();
		return ;
	}

    if (triggeredEvents & Events::Monitor::READ)
		::read(conn->m_socket.getSockFd(), &conn->actualResponse, 1);
}

void Client_FastNeverClose::disconnect()
{
    if (actualResponse == 200)
        m_clientManager.success++;
    else
        m_clientManager.fail++;

    m_monitor.release();
    ::close(m_socket.getSockFd());
	m_socket.setSockFd(Ws::FD_NONE);
}

//////////////////////////////////////////////////////////////////////////////////////////

Client_Math::Client_Math(int index, Events::Manager& eventManager, ClientManager<Client_Math>& clientManager) : 
	myIndex(index),
	request((unsigned char)index),
	expectedResponse((request + 3) % 256),
	actualResponse(0),
	hasWritten(false),
    m_monitor(eventManager),
    m_clientManager(clientManager) {}

int Client_Math::open()
{
    int sockfd = ::socket(m_socket.getBindInfo().family, m_socket.getBindInfo().socktype, m_socket.getBindInfo().proto);

    if (sockfd == -1)
        return -1;

    m_socket.setSockFd(sockfd);
    FileDescriptor::setNonBlocking(sockfd);

    m_monitor.acquire();
    Events::Subscription& sub = m_monitor.accessEvent();
    sub.setFd(m_socket.getSockFd());
    sub.setUser(this);
    sub.setMonitoredEvents(Events::Monitor::WRITE | Events::Monitor::ERROR | Events::Monitor::HANGUP);
    sub.setCallback(Client_Math::ReadWrite_Callback);

    return 1;
}

int Client_Math::connect()
{
    int res = ::connect(m_socket.getSockFd(), (struct sockaddr*)&m_socket.getBindInfo().addr, m_socket.getBindInfo().addrlen);

    if (res == -1 && errno != EINPROGRESS)
    {
        disconnect();
        m_clientManager.fail++;
    }
    m_monitor.subscribe(false);
    return 1;
}

ClientManager<Client_Math>& Client_Math::myManager()
{
	return m_clientManager;
}

void Client_Math::ReadWrite_Callback(Events::Subscription& sub)
{
    Client_Math* conn = reinterpret_cast<Client_Math*>(sub.accessUser());
	if (sub.getTriggeredEvents() & Events::Monitor::READ)
		conn->Read();
	if (sub.getTriggeredEvents() & Events::Monitor::WRITE)
		conn->Write();
}

/*
	Client subscribes itself for writting, at first.
	Once writing is done, changes the subscription to reading to get response from server.
*/
void Client_Math::Write()
{
	int triggeredEvents = m_monitor.accessEvent().getTriggeredEvents();

	if (triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP))
	{
		m_clientManager.fail++;
		disconnect();
		return ;
	}

	::write(m_socket.getSockFd(), &request, 1);
	hasWritten = true;
	Events::Subscription& sub = m_monitor.accessEvent();
	sub.setMonitoredEvents(Events::Monitor::READ | Events::Monitor::ERROR | Events::Monitor::HANGUP);
	m_monitor.modify(false);	
}

/*
	if read event and readBytes == 0, means server closed connection and there is nothing to read.
	So don't need to check for ERROR or HANGUP events.

	Transaction is done, unsubscribe all events.

	Check if the result is the expected one, mark on the ClientManager. Disconnect, all done.
*/

void Client_Math::Read()
{
	int bytesRead = ::read(m_socket.getSockFd(), &actualResponse, 1);

	if (bytesRead == 0)
	{
		m_clientManager.fail++;
		disconnect();
		return ;
	}

	m_monitor.unsubscribe(false);
	if (actualResponse == expectedResponse)
	{
		//std::cerr << "client success!!" << std::endl;
		m_clientManager.success++;
	}
	else
	{
		//std::cerr << "client fail!!" << std::endl;	
		m_clientManager.fail++;
	}
	disconnect();
}

void Client_Math::disconnect()
{
    m_monitor.release();
    ::close(m_socket.getSockFd());
	m_socket.setSockFd(Ws::FD_NONE);
}