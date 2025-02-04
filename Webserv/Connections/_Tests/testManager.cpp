
// targets
# include "../Manager/Manager.hpp"
# include "../Connection/Connection.hpp"

// dependenceis/helpers
# include "TestDependencies.hpp"
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




void testManager(int& testNumber)
{
    // global setup for all tests
    ThreadPool<10, 100> tp;
    Globals globals(NULL, NULL, NULL, NULL);
    ServerContext ctx;
    FakeHttp fakeHttp;
    ctx.setAppLayer(Ws::AppLayer::HTTP, &fakeHttp, &FakeHttp::InitConnection);

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

///////////////////////////////////////////////////////////

    try
    {
        TEST_INTRO(testNumber++);

        const int                   countListeners = 1;
        const int                   countMaxConnections = 1;
        Events::Manager             eventManager(countListeners + countMaxConnections, globals);
        std::vector<Ws::BindInfo>   bindAddresses(countListeners, (Ws::BindInfo){});

        prepareBindAddresses(bindAddresses, countListeners);
        Conn::Manager               manager(countMaxConnections, bindAddresses, eventManager, globals, ctx);


        TestConnector   connector;
        Socket          externalConnect;

        externalConnect.modifyBindInfo() = (Ws::BindInfo)
        {
            .appLayer = Ws::AppLayer::HTTP,
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

        TEST_PASSED_MSG("Manager: instantiation");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

///////////////////////////////////////////////////////////

/*
    try
    {
        TEST_INTRO(testNumber++);

        const int countListeners = 1;
        const int countMaxConnections = 1;
        const int countConnectors = 1;

        Events::Manager eventManager(countListeners + countMaxConnections, globals);

        std::vector<Ws::BindInfo> bindAddresses(countListeners);
        prepareBindAddresses(bindAddresses, countListeners);

        Conn::Manager manager(countMaxConnections, bindAddresses, eventManager, globals, ctx);

        EXPECT_EQUAL(manager.init(), true, "Manager::init() should initialize without issue");

        ClientManagerTask clientManagerTask(countConnectors, countListeners, globals);
        tp.addTask(clientManagerTask);

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
*/
}