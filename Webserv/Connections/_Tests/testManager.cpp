
// targets
# include "../Manager/Manager.hpp"
# include "../Connection/Connection.hpp"

// dependenceis/helpers
# include "../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../../Globals/Globals.hpp"
# include "../../Events/Manager/Manager.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

// C++ headers
#include <cstdlib>
#include <iostream>
#include <cerrno>

// C headers
#include <arpa/inet.h>
#include <unistd.h>


extern Ws::Sock::addr_in createSockAddr_in(const std::string& ip, const std::string& port);

class FakeHttp
{
    public:
        FakeHttp() {};
        ~FakeHttp() {};

        static void InitConnection(Conn::Connection& conn)
        {
            ++serveCount;
            conn.close();
        }
    static int serveCount;
};

int FakeHttp::serveCount = 0;

struct ClientManager;

int g_failCount = 0;
int g_successCount = 0;

struct TestConnector
{
    int     open()
    {
        int sockfd = ::socket(m_socket.getBindInfo().family, m_socket.getBindInfo().socktype, m_socket.getBindInfo().proto);
        
        if (sockfd == -1)
            return (-1);

        m_socket.setSockFd(sockfd);

        FileDescriptor::setNonBlocking(sockfd);

        m_monitor.acquire(*m_eventManager);
        Events::Subscription& sub = m_monitor.accessEvent();
        sub.setFd(m_socket.getSockFd());
        sub.setUser(this);
        sub.setMonitoredEvents(Events::Monitor::WRITE | Events::Monitor::ERROR | Events::Monitor::HANGUP);
        sub.setCallback(TestConnector::CallbackSuccess);
        

        return (1);
    }

    int    connect()
    {
        int res = ::connect(m_socket.getSockFd(), (struct sockaddr*)&m_socket.getBindInfo().addr, m_socket.getBindInfo().addrlen);

        if (res == -1 && errno != EINPROGRESS)
        {
            disconnect();
            g_failCount++;
        }
        m_monitor.subscribe(*m_eventManager, false);
        return (1);
    }

    static void CallbackSuccess(Events::Subscription& sub)
    {
        TestConnector* conn = reinterpret_cast<TestConnector*>(sub.accessUser());
        if (sub.getTriggeredEvents() & Events::Monitor::WRITE)
        {
            conn->disconnect();
            g_successCount++;
        }
        else
        {
            conn->disconnect();
            g_failCount++;
        }
    }

    void    disconnect()
    {
        m_monitor.release(*m_eventManager);
        ::close(m_socket.getSockFd());
    }
    
    Socket              m_socket;
    Monitor             m_monitor;
    Events::Manager*    m_eventManager;
};


struct ClientManager
{
    ClientManager(const size_t countConnectors, const size_t countListeners, Events::Manager& eventManager) :
        m_connectors(countConnectors)
    {
        for (size_t i = 0; i < countConnectors;  ++i)
        {
            m_connectors.emplace_back();
            m_connectors[i].m_socket.modifyBindInfo() = (Ws::BindInfo)
            {
                .appLayer = Ws::AppLayer::HTTP,
                .family = AF_INET,
                .socktype = SOCK_STREAM,
                .proto = IPPROTO_TCP,
                .addr = (Ws::Sock::union_addr){.sockaddr_in = createSockAddr_in("0.0.0.0", TestHelpers::to_string(8080 + (i % countListeners)))},
                .addrlen = sizeof(Ws::Sock::addr_in)
            };
            m_connectors[i].m_eventManager = &eventManager;
            m_connectors[i].open();
            
        }
    }

    ~ClientManager()
    {

    }

    void    ConnectAll()
    {
        for (size_t i = 0; i < m_connectors.size();  ++i)
        {
            if (m_connectors[i].connect() == -1)
            {
                std::cerr << "failed to connect" << std::endl;
            }
        }
    }

    HeapArray<TestConnector>    m_connectors;
};

void testManager(int& testNumber)
{
    try
    {
        TEST_INTRO(testNumber++);

        Globals globals(NULL, NULL, NULL, NULL);

        const int countListeners = 10;
        const int countMaxConnections = 100;
        const int countConnectors = 1000;

        Events::Manager eventManager(countListeners + countMaxConnections, globals);

        ServerContext ctx;
        FakeHttp fakeHttp;
        ctx.setAppLayer(Ws::AppLayer::HTTP, &fakeHttp, &FakeHttp::InitConnection);

        std::vector<Ws::BindInfo> bindAddresses(countListeners);

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

        Conn::Manager manager(countMaxConnections, bindAddresses, eventManager, globals, ctx);

        manager.init();

        int pid = fork();
        if (pid == 0)
        {
            {
                Events::Manager clientEvents(1000, globals);
                ClientManager clientManager(countConnectors, countListeners, clientEvents);

                clientManager.ConnectAll();

                while (clientEvents.getMonitoringCount() > 0)
                {
                    clientEvents.ProcessEvents(-1);
                }
                    
                std::cerr << "client manager done " << g_successCount<< "  " << g_failCount << std::endl;
            }
            
            exit(0);
        }
        

        

        while (FakeHttp::serveCount < countConnectors)
        {
            std::cout << "served: " << FakeHttp::serveCount << std::endl;
            eventManager.ProcessEvents(100);
        }
            
        
        manager.shutdown();

        EXPECT_EQUAL(FakeHttp::serveCount, countConnectors, "All connections should have been served");

        TEST_PASSED_MSG("Manager: instantiation");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
}