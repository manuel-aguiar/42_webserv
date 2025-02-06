
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

void    StressTestManager_MathProtocol(  int& testNumber,
                            ThreadPool<10, 100>& tp, 
                            Globals& globals, 
                            ServerContext& ctx,
                            const int input_countListeners,
                            const int input_countMaxConnections,
                            const int input_countConnectors,
                            const int input_clientTimeoutMs
                            )
{
    try
    {
        TEST_INTRO(testNumber++);

        const int countListeners = input_countListeners;
        const int countMaxConnections = input_countMaxConnections;
        const int countConnectors = input_countConnectors;
        const int clientTimeoutMs = input_clientTimeoutMs;

        std::string msg = "Manager: StressTest, MathProtocol: " 
                        + TestHelpers::to_string(countListeners) + " listeners, "
                        + TestHelpers::to_string(countMaxConnections) + " max connections, "
                        + TestHelpers::to_string(countConnectors) + " connectors, "
                        + TestHelpers::to_string(clientTimeoutMs) + "ms timeout";

        int maxEvents = countListeners + countMaxConnections;
		int maxFdsEstimate = (countConnectors + countListeners + countMaxConnections) * 1.2f;
        
        Events::Manager eventManager(maxEvents, globals, maxFdsEstimate);
        Server_MathModule fakeHttp(Ws::AppLayer::HTTP);
        ctx.setAppLayer(Ws::AppLayer::HTTP, &fakeHttp, &Server_MathModule::InitConnection);

        std::vector<Ws::BindInfo> bindAddresses(countListeners);
        prepareBindAddresses(bindAddresses, countListeners);

        Conn::Manager manager(countMaxConnections, bindAddresses, eventManager, globals, ctx);

        EXPECT_EQUAL(manager.init(), true, "Manager::init() should initialize without issue");

        int threadSuccessCount = -1;
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);

        ClientManagerTask<Client_Math> clientManagerTask(countConnectors, 
                                                        countListeners, 
                                                        countMaxConnections, 
                                                        globals, 
                                                        mutex, 
                                                        threadSuccessCount, 
                                                        clientTimeoutMs);
        tp.addTask(clientManagerTask);

        bool run = true;
        while (run)
        {
            eventManager.ProcessEvents(100);
             pthread_mutex_lock(&mutex);
             if (threadSuccessCount != -1)
                 run = false;
             pthread_mutex_unlock(&mutex);
        }

        tp.waitForCompletion();

        manager.shutdown();

        try
        {
            EXPECT_EQUAL(fakeHttp.serveCount, countConnectors, "All connections should have been served");
            EXPECT_EQUAL(threadSuccessCount, countConnectors, "Client should report that all clients received the right value");
        }
        catch(const std::exception& e)
        {
            throw std::runtime_error(msg + "; served " 
            + TestHelpers::to_string(threadSuccessCount) + "/" 
            + TestHelpers::to_string(countConnectors)
            + " connections; ");
        }

        
        

        TEST_PASSED_MSG(msg);
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
}


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
        const int clientTimeoutMs = 500;

        int maxEvents = countListeners + countMaxConnections;
		int maxFdsEstimate = (countConnectors + countListeners + countMaxConnections) * 1.2f;
        
        Events::Manager eventManager(maxEvents, globals, maxFdsEstimate);
        Server_FastCloseModule fakeHttp(Ws::AppLayer::HTTP);
        ctx.setAppLayer(Ws::AppLayer::HTTP, &fakeHttp, &Server_FastCloseModule::InitConnection);

        std::vector<Ws::BindInfo> bindAddresses(countListeners);
        prepareBindAddresses(bindAddresses, countListeners);

        Conn::Manager manager(countMaxConnections, bindAddresses, eventManager, globals, ctx);

        EXPECT_EQUAL(manager.init(), true, "Manager::init() should initialize without issue");

        int threadSuccessCount = -1;
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);

        ClientManagerTask<Client_FastNeverClose> clientManagerTask(countConnectors, 
                                                                    countListeners,
                                                                    countMaxConnections,  
                                                                    globals, 
                                                                    mutex, 
                                                                    threadSuccessCount, 
                                                                    clientTimeoutMs);
        tp.addTask(clientManagerTask);

        bool run = true;
        while (run)
        {
            eventManager.ProcessEvents(100);
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

        int maxEvents = countListeners + countMaxConnections;
		int maxFdsEstimate = (countConnectors + countListeners + countMaxConnections) * 1.2f;

        Events::Manager eventManager(maxEvents, globals, maxFdsEstimate);
        Server_NeverCloseModule fakeHttp(Ws::AppLayer::HTTP);
        ctx.setAppLayer(Ws::AppLayer::HTTP, &fakeHttp, &Server_NeverCloseModule::InitConnection);

        std::vector<Ws::BindInfo> bindAddresses(countListeners);
        prepareBindAddresses(bindAddresses, countListeners);

        Conn::Manager manager(countMaxConnections, bindAddresses, eventManager, globals, ctx);

        EXPECT_EQUAL(manager.init(), true, "Manager::init() should initialize without issue");

        int threadSuccessCount = -1;
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);

        ClientManagerTask<Client_FastNeverClose> clientManagerTask( countConnectors, 
                                                                    countListeners,
                                                                    countMaxConnections, 
                                                                    globals, 
                                                                    mutex, 
                                                                    threadSuccessCount, 
                                                                    clientTimeoutMs);
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
        const int clientTimeoutMs = 5000;

        std::string msg = "Manager: MathProtocol, two way communication, simple math request and response";

        int maxEvents = countListeners + countMaxConnections;
		int maxFdsEstimate = (countConnectors + countListeners + countMaxConnections) * 1.2f;
        
        Events::Manager eventManager(maxEvents, globals, maxFdsEstimate);
        Server_MathModule fakeHttp(Ws::AppLayer::HTTP);
        ctx.setAppLayer(Ws::AppLayer::HTTP, &fakeHttp, &Server_MathModule::InitConnection);

        std::vector<Ws::BindInfo> bindAddresses(countListeners);
        prepareBindAddresses(bindAddresses, countListeners);

        Conn::Manager manager(countMaxConnections, bindAddresses, eventManager, globals, ctx);

        EXPECT_EQUAL(manager.init(), true, "Manager::init() should initialize without issue");

        int threadSuccessCount = -1;
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);

        ClientManagerTask<Client_Math> clientManagerTask(countConnectors, 
                                                        countListeners, 
                                                        countMaxConnections, 
                                                        globals, 
                                                        mutex, 
                                                        threadSuccessCount, 
                                                        clientTimeoutMs);
        tp.addTask(clientManagerTask);

        bool run = true;
        while (run)
        {
            eventManager.ProcessEvents(100);
             pthread_mutex_lock(&mutex);
             if (threadSuccessCount != -1)
                 run = false;
             pthread_mutex_unlock(&mutex);
        }

        tp.waitForCompletion();

        manager.shutdown();

        EXPECT_EQUAL(fakeHttp.serveCount, countConnectors, "All connections should have been served");
        EXPECT_EQUAL(threadSuccessCount, countConnectors, "Client should report that all clients received the right value");

        TEST_PASSED_MSG(msg);
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
/////////////////////////////////////////////////////////////////////////////////
    //Stress tests
    std::cout << TEST_CLR_BROWN << "\tStress tests (failure is not an error)" << TEST_CLR_RESET << std::endl;
    StressTestManager_MathProtocol(testNumber, tp, globals, ctx, 1, 10, 100, 5000);
    StressTestManager_MathProtocol(testNumber, tp, globals, ctx, 10, 10, 100, 5000);
    StressTestManager_MathProtocol(testNumber, tp, globals, ctx, 10, 100, 1000, 1000);
    StressTestManager_MathProtocol(testNumber, tp, globals, ctx, 10, 100, 10000, 2000);
    StressTestManager_MathProtocol(testNumber, tp, globals, ctx, 10, 100, 10000, 5000);
    StressTestManager_MathProtocol(testNumber, tp, globals, ctx, 100, 100, 10000, 2000);
    StressTestManager_MathProtocol(testNumber, tp, globals, ctx, 100, 1000, 10000, 3000);
    StressTestManager_MathProtocol(testNumber, tp, globals, ctx, 10, 5000, 10000, 3000);
    StressTestManager_MathProtocol(testNumber, tp, globals, ctx, 1, 5000, 10000, 5000);
}

