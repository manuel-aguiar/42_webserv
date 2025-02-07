
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
                            ThreadPoolGeneric& tp, 
                            Globals& globals, 
                            ServerContext& ctx,
                            const int input_countListeners,
                            const int input_countMaxConnections,
                            const int input_countConnectors,
                            const int input_clientTimeoutMs
                            );


void testStressTest(int& testNumber)
{
    ThreadPoolHeap      tpHeap(3, 10);
    ThreadPool<4, 5>    tp;
    Globals             globals(NULL, NULL, NULL, NULL);
    ServerContext       ctx;
    
    //Stress tests
    std::cout << TEST_CLR_BROWN << "\n\tStress tests (failure is not an error)" << TEST_CLR_RESET << std::endl;
    StressTestManager_MathProtocol(testNumber, tp       , globals, ctx, 1, 10, 100, 5000);
    StressTestManager_MathProtocol(testNumber, tpHeap   , globals, ctx, 10, 10, 100, 5000);
    StressTestManager_MathProtocol(testNumber, tp       , globals, ctx, 10, 100, 1000, 1000);
    StressTestManager_MathProtocol(testNumber, tpHeap   , globals, ctx, 10, 100, 10000, 2000);
    StressTestManager_MathProtocol(testNumber, tp       , globals, ctx, 10, 100, 10000, 5000);
    StressTestManager_MathProtocol(testNumber, tpHeap   , globals, ctx, 100, 100, 10000, 2000);
    StressTestManager_MathProtocol(testNumber, tp       , globals, ctx, 100, 1000, 10000, 3000);
    StressTestManager_MathProtocol(testNumber, tpHeap   , globals, ctx, 10, 5000, 10000, 3000);
    StressTestManager_MathProtocol(testNumber, tp       , globals, ctx, 1, 5000, 10000, 5000);
    StressTestManager_MathProtocol(testNumber, tpHeap   , globals, ctx, 1, 2000, 10000, 5000);
    StressTestManager_MathProtocol(testNumber, tp       , globals, ctx, 1, 1000, 10000, 5000);   
}

void    StressTestManager_MathProtocol(  int& testNumber,
                            ThreadPoolGeneric& tp, 
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

        pthread_mutex_destroy(&mutex);

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
