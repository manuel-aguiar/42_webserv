
// Project headers
# include "../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../Accepter/Accepter.hpp"
# include "../../Events/Manager/Manager.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../Globals/Globals.hpp"

# include "../Socket/Socket.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

// C++ headers
#include <cstdlib>
#include <iostream>

// C headers
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>

extern Ws::Sock::addr_in createSockAddr_in(const std::string& ip, const std::string& port);

struct AccepterCallbackBundle
{
    Accepter& accepter;
    Socket& listenInfo;
    Socket& internalConnect;
};

void  AccepterCallback(Events::Subscription& subs)
{
    AccepterCallbackBundle* bundle = reinterpret_cast<AccepterCallbackBundle*>(subs.accessUser());
    bundle->accepter.accept(bundle->listenInfo, bundle->internalConnect);
}

struct TestConnector
{
    int    connect(Socket& socket)
    {
        m_socket = ::socket(socket.getBindInfo().family, socket.getBindInfo().socktype, socket.getBindInfo().proto);
        if (m_socket == -1)
            return (-1);

        // TestConnector will block until it is accepted
        return(::connect(m_socket, (struct sockaddr*)&socket.getBindInfo().addr, socket.getBindInfo().addrlen));
    }

    void    disconnect(Socket& socket)
    {
        ::close(m_socket);
        ::close(socket.getSockFd());
    }

    Ws::Sock::fd m_socket;
};

void testAccepter(int& testNumber)
{
    Globals globals(NULL, NULL, NULL, NULL);
//////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);

        Accepter accepter(10);

        TEST_PASSED_MSG("Accepter: instantiation");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
//////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);

        Accepter        accepter(10);
        TestConnector   connector;
        Socket          listenInfo;
        Socket          externalConnect;
        Socket          internalConnect;

        listenInfo.modifyBindInfo() = (Ws::BindInfo)
        {
            .appLayer = Ws::AppLayer::HTTP,
            .family = AF_INET,
            .socktype = SOCK_STREAM,
            .proto = IPPROTO_TCP,
            .addr = (Ws::Sock::union_addr){.sockaddr_in = createSockAddr_in("0.0.0.0", "8080")},
            .addrlen = sizeof(Ws::Sock::addr_in)
        };

        externalConnect.modifyBindInfo() = (Ws::BindInfo)
        {
            .appLayer = Ws::AppLayer::HTTP,
            .family = AF_INET,
            .socktype = SOCK_STREAM,
            .proto = IPPROTO_TCP,
            .addr = (Ws::Sock::union_addr){.sockaddr_in = createSockAddr_in("127.0.0.1", "8080")},
            .addrlen = sizeof(Ws::Sock::addr_in)
        };

        EXPECT_EQUAL(listenInfo.getSockFd() == Ws::FD_NONE, true, "Sockfd shouldn't be set at this point");
        EXPECT_EQUAL(accepter.open(listenInfo), 1, "Accepter::open() failed");
        EXPECT_EQUAL(listenInfo.getSockFd() != Ws::FD_NONE, true, "Accepter::open() failed, socket fd not set");

        std::cout.flush();
        int pid = fork();
        if (!pid)
        {
            // connect and disconnect
            connector.connect(externalConnect);
            connector.disconnect(externalConnect);
            accepter.close(listenInfo);
            ::close(internalConnect.getSockFd());
            exit(0);
        }

        //ok i'll use the event manager here, easier than sleeps to wait for the child to actually try to connect
        {
            Events::Manager eventManager(1, globals);
            Events::Subscription* subs = eventManager.acquireSubscription();
            subs->setFd(listenInfo.getSockFd());
            subs->setUser(&accepter);
            subs->setMonitoredEvents(Events::Monitor::READ | Events::Monitor::ERROR | Events::Monitor::HANGUP | Events::Monitor::EDGE_TRIGGERED);
            AccepterCallbackBundle bundle = {.accepter = accepter, .listenInfo = listenInfo, .internalConnect = internalConnect};
            subs->setCallback(AccepterCallback);
            subs->setUser(&bundle);
            eventManager.startMonitoring(*subs, false);  
            eventManager.ProcessEvents(-1);
            eventManager.stopMonitoring(*subs, false);
            eventManager.returnSubscription(*subs);
        }

        

        // checking the accepter recorded the data from the incoming connection
        EXPECT_EQUAL(internalConnect.getSockFd() != -1, true, "Accepter::accept(), should have set the socket fd");
        EXPECT_EQUAL(internalConnect.getBindInfo().family, externalConnect.getBindInfo().family, "family: InternalConnect and ExternalConnect info must be the same");
        EXPECT_EQUAL(internalConnect.getBindInfo().socktype, externalConnect.getBindInfo().socktype, "socktype: InternalConnect and ExternalConnect info must be the same");
        EXPECT_EQUAL(internalConnect.getBindInfo().proto, externalConnect.getBindInfo().proto, "proto: InternalConnect and ExternalConnect info must be the same");

        accepter.close(listenInfo);
        ::close(internalConnect.getSockFd());
        ::waitpid(pid, NULL, 0);

        TEST_PASSED_MSG("Accepter: accepts and closes connections");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
}