
// target
# include "../Listener/Listener.hpp"

// Project headers
# include "_TestDependencies.hpp"
# include "_ClientProtos.hpp"
# include "../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../../Events/Manager/Manager.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../Globals/Globals.hpp"
# include "../../../Toolkit/ThreadPool/ThreadPool.hpp"
# include "../ConnInfo/ConnInfo.hpp"
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
    Listener& listener;
    ConnInfo& internalConnect;
};

void  AccepterCallback(Events::Subscription& subs)
{
    AccepterCallbackBundle* bundle = reinterpret_cast<AccepterCallbackBundle*>(subs.accessUser());
    bundle->listener.accept(bundle->internalConnect);
}

void testListener(int& testNumber)
{

    ThreadPool<10, 100> tp;
    Globals globals(NULL, NULL, NULL, NULL);
//////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);

        Listener listener((Ws::BindInfo){});

        TEST_PASSED_MSG("Listener: instantiation");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
//////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);


        Ws::BindInfo    listenInfo =(Ws::BindInfo)
        {
            .appLayer = Ws::AppLayer::HTTP,
            .backlog = 128,
            .family = AF_INET,
            .socktype = SOCK_STREAM,
            .proto = IPPROTO_TCP,
            .addr = (Ws::Sock::union_addr){.sockaddr_in = createSockAddr_in("0.0.0.0", "8080")},
            .addrlen = sizeof(Ws::Sock::addr_in)                
        };

        Listener            listener(listenInfo);
        TestConnector       connector;
        ConnInfo            externalConnect;
        ConnInfo            internalConnect;

        externalConnect.bind = (Ws::BindInfo)
        {
            .appLayer = Ws::AppLayer::HTTP,
            .backlog = 128,
            .family = AF_INET,
            .socktype = SOCK_STREAM,
            .proto = IPPROTO_TCP,
            .addr = (Ws::Sock::union_addr){.sockaddr_in = createSockAddr_in("127.0.0.1", "8080")},
            .addrlen = sizeof(Ws::Sock::addr_in)
        };


        EXPECT_EQUAL(listener.getConnInfo().sockfd == Ws::FD_NONE, true, "Sockfd shouldn't be set at this point");
        EXPECT_EQUAL(listener.open(), 1, "Listener::open() failed");
        EXPECT_EQUAL(listener.getConnInfo().sockfd != Ws::FD_NONE, true, "Listener::open() failed, socket fd not set");

        ClientTask task(connector, externalConnect);
        tp.addTask(task);
        

        //ok i'll use the event manager here, easier than sleeps to wait for the child to actually try to connect
        {
            Events::Manager eventManager(1, globals);
            Events::Subscription* subs = eventManager.acquireSubscription();
            subs->setFd(listener.getConnInfo().sockfd);
            subs->setUser(&listener);
            subs->setMonitoredEvents(Events::Monitor::READ | Events::Monitor::ERROR | Events::Monitor::HANGUP | Events::Monitor::EDGE_TRIGGERED);
            AccepterCallbackBundle bundle = {.listener = listener, .internalConnect = internalConnect};
            subs->setCallback(AccepterCallback);
            subs->setUser(&bundle);
            eventManager.startMonitoring(*subs, false);  
            eventManager.ProcessEvents(-1);
            eventManager.stopMonitoring(*subs, false);
            eventManager.returnSubscription(*subs);
        }
        tp.waitForCompletion();
        connector.disconnect();
        

        // checking the listener recorded the data from the incoming connection
        EXPECT_EQUAL(internalConnect.sockfd != Ws::FD_NONE, true, "Listener::accept(), should have set the socket fd");
        EXPECT_EQUAL(internalConnect.bind.family, externalConnect.bind.family, "family: InternalConnect and ExternalConnect info must be the same");
        EXPECT_EQUAL(internalConnect.bind.socktype, externalConnect.bind.socktype, "socktype: InternalConnect and ExternalConnect info must be the same");
        EXPECT_EQUAL(internalConnect.bind.proto, externalConnect.bind.proto, "proto: InternalConnect and ExternalConnect info must be the same");
        EXPECT_EQUAL(internalConnect.bind.addrlen, externalConnect.bind.addrlen, "addrlen: InternalConnect and ExternalConnect info must be the same");
        EXPECT_EQUAL(internalConnect.bind.addr.sockaddr_in.sin_addr.s_addr, externalConnect.bind.addr.sockaddr_in.sin_addr.s_addr, "addr: InternalConnect and ExternalConnect info must be the same");
        EXPECT_EQUAL(internalConnect.bind.addr.sockaddr_in.sin_port, externalConnect.bind.addr.sockaddr_in.sin_port, "port: InternalConnect and ExternalConnect info must be the same");

        listener.close();
        ::close(internalConnect.sockfd);

        TEST_PASSED_MSG("Listener: accepts and closes connections");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
}