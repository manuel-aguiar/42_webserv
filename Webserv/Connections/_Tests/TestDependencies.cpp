

// Prooject headers
#include "TestDependencies.hpp"
#include "../Socket/Socket.hpp"
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../TimerTracker/Timer/Timer.hpp"
# include "../../Globals/Globals.hpp"
# include "../../Events/Manager/Manager.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

// C headers
#include <unistd.h>     // For close()
#include <sys/socket.h> // For socket(), connect()
#include <arpa/inet.h>  // For inet_pton()

//C++ headers
#include <vector>
#include <cerrno>

int TestConnector::connect(Socket& socket)
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

ClientTask::ClientTask(TestConnector& connector, Socket& socket)
    : m_connector(connector), m_socket(socket) {}

void ClientTask::execute()
{
    m_connector.connect(m_socket);
}


ManagedClient::ManagedClient() : received(0) {}

int ManagedClient::open()
{
    int sockfd = ::socket(m_socket.getBindInfo().family, m_socket.getBindInfo().socktype, m_socket.getBindInfo().proto);

    if (sockfd == -1)
        return -1;

    m_socket.setSockFd(sockfd);
    FileDescriptor::setNonBlocking(sockfd);

    m_monitor.acquire(*m_eventManager);
    Events::Subscription& sub = m_monitor.accessEvent();
    sub.setFd(m_socket.getSockFd());
    sub.setUser(this);
    sub.setMonitoredEvents(Events::Monitor::READ | Events::Monitor::ERROR | Events::Monitor::HANGUP);
    sub.setCallback(ManagedClient::CallbackSuccess);

    return 1;
}

int ManagedClient::connect()
{
    int res = ::connect(m_socket.getSockFd(), (struct sockaddr*)&m_socket.getBindInfo().addr, m_socket.getBindInfo().addrlen);

    if (res == -1 && errno != EINPROGRESS)
    {
        disconnect();
        m_clientManager->fail++;
    }
    m_monitor.subscribe(*m_eventManager, false);
    return 1;
}

ClientManager* ManagedClient::myManager()
{
	return m_clientManager;
}

void ManagedClient::CallbackSuccess(Events::Subscription& sub)
{
    ManagedClient* conn = reinterpret_cast<ManagedClient*>(sub.accessUser());
	int triggeredEvents = sub.getTriggeredEvents();
	

	if (triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP) && conn->received == 0)
	{
		conn->disconnect();
		conn->myManager()->fail++;
		return ;
	}

    if (triggeredEvents & Events::Monitor::READ)
    {
		::read(conn->m_socket.getSockFd(), &conn->received, 1);

        conn->disconnect();
		if (conn->received == 200)
		{
			//std::cerr << "\t client successfully connected" << std::endl;
        	conn->myManager()->success++;
		}
		else
			conn->myManager()->fail++;
    }
}

void ManagedClient::disconnect()
{
    m_monitor.release(*m_eventManager);
    ::close(m_socket.getSockFd());
	m_socket.setSockFd(Ws::FD_NONE);
}


ClientManager::ClientManager(const size_t countConnectors, const size_t countListeners, Events::Manager& eventManager)
    : success(0), fail(0), m_connectors(countConnectors)
{
    for (size_t i = 0; i < countConnectors; ++i)
    {
        m_connectors.emplace_back();
        m_connectors[i].m_socket.modifyBindInfo() = (Ws::BindInfo)
        {
            .appLayer = Ws::AppLayer::HTTP,
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

ClientManager::~ClientManager()
{
	for (size_t i = 0; i < m_connectors.size(); ++i)
	{
		if (m_connectors[i].m_socket.getSockFd() != Ws::FD_NONE)
		{
			m_connectors[i].disconnect();
			fail++;
		}
	}
}

void ClientManager::ConnectAll()
{
    for (size_t i = 0; i < m_connectors.size(); ++i)
    {
        if (m_connectors[i].connect() == -1)
        {
            //std::cerr << "failed to connect" << std::endl;
        }
    }
}

ClientManagerTask::ClientManagerTask(const size_t countConnectors, const size_t countListeners, Globals& globals, pthread_mutex_t& mutex, int& exitSignal)
    : countConnectors(countConnectors), countListeners(countListeners), globals(globals), mutex(mutex), exitSignal(exitSignal) {}

void ClientManagerTask::execute()
{
    Events::Manager clientEvents(1000, globals);
    ClientManager clientManager(countConnectors, countListeners, clientEvents);
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

Ws::Sock::addr_in createSockAddr_in(const std::string& ip, const std::string& port)
{
    Ws::Sock::addr_in addr = (Ws::Sock::addr_in){};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(std::atoi(port.c_str()));

    if (ip.empty() || ip == "0.0.0.0")
        addr.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0
    else
        inet_pton(AF_INET, ip.c_str(), &(addr.sin_addr));

    return (addr);
}

void    prepareBindAddresses(std::vector<Ws::BindInfo>& bindAddresses, const size_t countListeners)
{
    bindAddresses.resize(countListeners);
    for (size_t i = 0; i < bindAddresses.size(); ++i)
    {
        bindAddresses[i] = (Ws::BindInfo)
        {
            .appLayer = Ws::AppLayer::HTTP,
            .family = AF_INET,
            .socktype = SOCK_STREAM,
            .proto = IPPROTO_TCP,
            .addr = (Ws::Sock::union_addr){.sockaddr_in = createSockAddr_in("0.0.0.0", TestHelpers::to_string(8080 + i))},
            .addrlen = sizeof(Ws::Sock::addr_in)
        };
    }
}