
// targets
# include "../Manager/Manager.hpp"
# include "../Connection/Connection.hpp"

// dependenceis/helpers
# include "_TestDependencies.hpp"
# include "_TestClientManager.tpp"
# include "_ClientProtos.hpp"
# include "_ServerProtos.hpp"

# include "../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../../../Toolkit/ThreadPool/ThreadPool.hpp"
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
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



void testManager(int& testNumber)
{
    // global setup for all tests
    ThreadPool<10, 100> tp;
    Globals globals(NULL, NULL, NULL, NULL);
    ServerContext ctx;
    

//////////////////////////////////////////////////

    try
    {
        TEST_INTRO(testNumber++);

        const int                   countListeners = 10;
        const int                   countMaxConnections = 10;
        Events::Manager             eventManager(countListeners + countMaxConnections, globals);
        std::vector<Ws::BindInfo>   bindAddresses;

        prepareBindAddresses(bindAddresses, countListeners);
        
        Conn::Manager               manager(countMaxConnections, bindAddresses, eventManager, globals, ctx);


        EXPECT_EQUAL(manager.init(), true, "Manager::init() should initialize without issue");

        manager.shutdown();

        TEST_PASSED_MSG("Manager: instantiation");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);

        const int                   countListeners = 1;
        const int                   countMaxConnections = 1;
        Events::Manager             eventManager(countListeners + countMaxConnections, globals);
        std::vector<Ws::BindInfo>   bindAddresses(countListeners, (Ws::BindInfo){});

        prepareBindAddresses(bindAddresses, countListeners);
        Conn::Manager               manager(countMaxConnections, bindAddresses, eventManager, globals, ctx);

        EXPECT_EQUAL(manager.init(), true, "Manager::init() should initialize without issue");

        manager.shutdown();

        TEST_PASSED_MSG("Manager: instantiation");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);

        Server_FastCloseModule fakeHttp(Ws::AppLayer::HTTP);
        ctx.setAppLayer(Ws::AppLayer::HTTP, &fakeHttp, &Server_FastCloseModule::InitConnection);

        const int                   countListeners = 10;
        const int                   countMaxConnections = 1;
        Events::Manager             eventManager(countListeners + countMaxConnections, globals);
        std::vector<Ws::BindInfo>   bindAddresses(countListeners, (Ws::BindInfo){});

        prepareBindAddresses(bindAddresses, countListeners);
        Conn::Manager               manager(countMaxConnections, bindAddresses, eventManager, globals, ctx);

        EXPECT_EQUAL(manager.init(), true, "Manager::init() should initialize without issue");

        manager.shutdown();

        TEST_PASSED_MSG("Manager: instantiation with different starting values");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

///////////////////////////////////////////////////////////

    try
    {
        TEST_INTRO(testNumber++);

        const int                   countListeners = 1;
        const int                   countMaxConnections = 1;
        Events::Manager             eventManager(countListeners + countMaxConnections, globals);
        std::vector<Ws::BindInfo>   bindAddresses(countListeners, (Ws::BindInfo){});
        Server_FastCloseModule fakeHttp(Ws::AppLayer::HTTP);
        ctx.setAppLayer(Ws::AppLayer::HTTP, &fakeHttp, &Server_FastCloseModule::InitConnection);

        prepareBindAddresses(bindAddresses, countListeners);
        Conn::Manager        manager(countMaxConnections, bindAddresses, eventManager, globals, ctx);


        TestConnector   connector;
        Socket          externalConnect;

        externalConnect.modifyBindInfo() = (Ws::BindInfo)
        {
            .appLayer = Ws::AppLayer::HTTP,
            .backlog = 128,
            .family = AF_INET,
            .socktype = SOCK_STREAM,
            .proto = IPPROTO_TCP,
            .addr = (Ws::Sock::union_addr){.sockaddr_in = createSockAddr_in("127.0.0.1", "8080")},
            .addrlen = sizeof(Ws::Sock::addr_in)
        };

        EXPECT_EQUAL(manager.init(), true, "Manager::init() should initialize without issue");

        ClientTask task(connector, externalConnect);
        tp.addTask(task);

        eventManager.ProcessEvents(-1);

        tp.waitForCompletion();
        connector.disconnect();

        manager.shutdown();

        TEST_PASSED_MSG("Manager: passing a single client and see if it connects");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

///////////////////////////////////////////////////////////


    try
    {
        TEST_INTRO(testNumber++);

        const int countListeners = 10;
        const int countMaxConnections = 10;
        const int countConnectors = 100;
        const int clientTimeoutMs = 100;

        Events::Manager eventManager(countListeners + countMaxConnections, globals);
        Server_FastCloseModule fakeHttp(Ws::AppLayer::HTTP);
        ctx.setAppLayer(Ws::AppLayer::HTTP, &fakeHttp, &Server_FastCloseModule::InitConnection);

        std::vector<Ws::BindInfo> bindAddresses(countListeners);
        prepareBindAddresses(bindAddresses, countListeners);

        Conn::Manager manager(countMaxConnections, bindAddresses, eventManager, globals, ctx);

        EXPECT_EQUAL(manager.init(), true, "Manager::init() should initialize without issue");

        int threadSuccessCount = -1;
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);

        ClientManagerTask<Client_FastNeverClose> clientManagerTask(countConnectors, countListeners, globals, mutex, threadSuccessCount, clientTimeoutMs);
        tp.addTask(clientManagerTask);

        bool run = true;
        while (run)
        {
            int wait = eventManager.ProcessEvents(100);
            (void)wait;
             pthread_mutex_lock(&mutex);
             if (threadSuccessCount != -1)
                 run = false;
             pthread_mutex_unlock(&mutex);
        }

        tp.waitForCompletion();

        manager.shutdown();

        EXPECT_EQUAL(fakeHttp.serveCount, countConnectors, "All connections should have been served");

        TEST_PASSED_MSG("Manager: Protocol FastClose to test connection handling");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

///////////////////////////////////////////////////////////


    try
    {
        TEST_INTRO(testNumber++);

        const int countListeners = 10;
        const int countMaxConnections = 150;
        const int countConnectors = 100;
        const int clientTimeoutMs = 2000;

        Events::Manager eventManager(countListeners + countMaxConnections, globals);
        Server_NeverCloseModule fakeHttp(Ws::AppLayer::HTTP);
        ctx.setAppLayer(Ws::AppLayer::HTTP, &fakeHttp, &Server_NeverCloseModule::InitConnection);

        std::vector<Ws::BindInfo> bindAddresses(countListeners);
        prepareBindAddresses(bindAddresses, countListeners);

        Conn::Manager manager(countMaxConnections, bindAddresses, eventManager, globals, ctx);

        EXPECT_EQUAL(manager.init(), true, "Manager::init() should initialize without issue");

        int threadSuccessCount = -1;
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);

        ClientManagerTask<Client_FastNeverClose> clientManagerTask(countConnectors, countListeners, globals, mutex, threadSuccessCount, clientTimeoutMs);
        tp.addTask(clientManagerTask);

        ::sleep(1);

        eventManager.ProcessEvents(100);

        manager.shutdown();

        tp.waitForCompletion();

        EXPECT_EQUAL(fakeHttp.serveCount, countConnectors, "All connections should have been served");

        TEST_PASSED_MSG("Manager: Protocol NeverClose -> testing connection ForceClose from Manager shutdown");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

///////////////////////////////////////////////////////////////////

    try
    {
        TEST_INTRO(testNumber++);

        const int countListeners = 10;
        const int countMaxConnections = 100;
        const int countConnectors = 1000;
        const int clientTimeoutMs = 1000;

        Events::Manager eventManager(countListeners + countMaxConnections, globals);
        Server_MathModule fakeHttp(Ws::AppLayer::HTTP);
        ctx.setAppLayer(Ws::AppLayer::HTTP, &fakeHttp, &Server_MathModule::InitConnection);

        std::vector<Ws::BindInfo> bindAddresses(countListeners);
        prepareBindAddresses(bindAddresses, countListeners);

        Conn::Manager manager(countMaxConnections, bindAddresses, eventManager, globals, ctx);

        EXPECT_EQUAL(manager.init(), true, "Manager::init() should initialize without issue");

        int threadSuccessCount = -1;
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);

        ClientManagerTask<Client_Math> clientManagerTask(countConnectors, countListeners, globals, mutex, threadSuccessCount, clientTimeoutMs);
        tp.addTask(clientManagerTask);

        bool run = true;
        while (run)
        {
            int wait = eventManager.ProcessEvents(100);
            (void)wait;
            //std::cout << "events received: " << wait;
            //std::cout  << ", monitoring: " << eventManager.getMonitoringCount();
            //std::cout  << ", current served: " << fakeHttp.serveCount << " out of " << countConnectors << std::endl;
             pthread_mutex_lock(&mutex);
             if (threadSuccessCount != -1)
                 run = false;
             pthread_mutex_unlock(&mutex);
        }

        tp.waitForCompletion();

        manager.shutdown();

        EXPECT_EQUAL(fakeHttp.serveCount, countConnectors, "All connections should have been served");
        EXPECT_EQUAL(threadSuccessCount, countConnectors, "Client should report that all clients received the right value");

        TEST_PASSED_MSG("Manager: Protocol Math, client sends a byte, server transforms and sends back, client checks if response matches");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

}