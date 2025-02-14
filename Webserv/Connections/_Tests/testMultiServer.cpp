
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

void    StressTest_MultiServer( int& testNumber,
                                ThreadPoolGeneric& serverTP, 
                                ThreadPoolGeneric& ClientTP,
                                Globals& globals, 
                                const int input_serverCount,
                                const int input_startingPort,
                                const int input_countListeners,
                                const int input_countMaxConnections,
                                const int input_countConnectors,
                                const int input_clientTimeoutMs);

void    MathProtocolMultiServer(
                                        int& exitSignal,  
                                        Globals& globals, 
                                        const int input_startingPort,
                                        pthread_mutex_t& exitMutex,
                                        const int input_countListeners,
                                        const int input_countMaxConnections,
                                        const int input_countConnectors
);

struct MultiServerTask : public IThreadTask
{
    int& exitSignal;
    Globals& globals;
    const int input_startingPort;
    pthread_mutex_t& exitMutex;
    const int input_countListeners;
    const int input_countMaxConnections;
    const int input_countConnectors;
    
    MultiServerTask(
        int& exitSignal,
        Globals& globals, 
        const int input_startingPort,
        pthread_mutex_t& exitMutex,
        const int input_countListeners,
        const int input_countMaxConnections,
        const int input_countConnectors
    ) : 
        exitSignal(exitSignal),
        globals(globals),
        input_startingPort(input_startingPort),
        exitMutex(exitMutex),
        input_countListeners(input_countListeners),
        input_countMaxConnections(input_countMaxConnections),
        input_countConnectors(input_countConnectors)
    {}

    void execute()
    {
        MathProtocolMultiServer(exitSignal, 
                                globals, 
                                input_startingPort, 
                                exitMutex, 
                                input_countListeners, 
                                input_countMaxConnections, 
                                input_countConnectors);
    }
};


void testMultiServer(int& testNumber)
{
    ThreadPoolHeap      serverTp(20,50);
    ThreadPoolHeap      clientTp(1, 20);
    Globals             globals(NULL, NULL, NULL, NULL);

    //Stress tests
    std::cout << TEST_CLR_BROWN << "\n\tMultiServer Stress tests (failure is not an error)" << TEST_CLR_RESET << std::endl;

    //StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 10, 8080, 6, 100, 10000, 2000);
    //StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 5, 8080, 1, 10, 100, 5000);
    //StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 10, 8080, 1, 100, 10000, 5000);
    //StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 10, 8080, 2, 100, 10000, 5000);

    //StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 1, 8080, 1, 10, 100, 5000);
    //StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 1, 8080, 10, 10, 100, 5000);
    //StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 1, 8080, 10, 100, 1000, 1000);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals,  1, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals,  2, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals,  3, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals,  4, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals,  5, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals,  6, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals,  7, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals,  8, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals,  9, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 10, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 11, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 12, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 13, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 14, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 15, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 16, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 17, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 18, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 19, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 20, 8080, 10, 100, 10000, 500);
    //StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 1, 8080, 10, 100, 10000, 5000);
    //StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 1, 8080, 100, 100, 10000, 2000);
    //StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 1, 8080, 100, 1000, 10000, 3000);
    //StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 1, 8080, 10, 5000, 10000, 3000);
    //StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 1, 8080, 1, 5000, 10000, 5000);
    //StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 1, 8080, 1, 2000, 10000, 5000);
    //StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 1, 8080, 1, 1000, 10000, 5000); 

}

void    StressTest_MultiServer( int& testNumber,
                                ThreadPoolGeneric& serverTP, 
                                ThreadPoolGeneric& ClientTP,
                                Globals& globals, 
                                const int input_serverCount,
                                const int input_startingPort,
                                const int input_countListeners,
                                const int input_countMaxConnections,
                                const int input_countConnectors,
                                const int input_clientTimeoutMs)
{
    int                 threadSuccessCount = -1;
    pthread_mutex_t     exitMutex;

    std::string msg = "Manager: MultiServer, MathProtocol: " 
                + TestHelpers::to_string(input_serverCount) + " servers, "
                + TestHelpers::to_string(input_countListeners) + " listeners, "
                + TestHelpers::to_string(input_countMaxConnections) + " max connections, "
                + TestHelpers::to_string(input_countConnectors) + " connectors, "
                + TestHelpers::to_string(input_clientTimeoutMs) + "ms timeout";

    pthread_mutex_init(&exitMutex, NULL);

    ClientManagerTask<Client_Math> clientManagerTask(input_countConnectors, 
                                                input_countListeners, 
                                                input_countMaxConnections, 
                                                globals, 
                                                exitMutex, 
                                                threadSuccessCount, 
                                                input_clientTimeoutMs,
                                                input_startingPort);

    std::vector<MultiServerTask> tasks;

    tasks.reserve(input_serverCount);
    for (int i = 0; i < input_serverCount; i++)
    {
        tasks.push_back(MultiServerTask(threadSuccessCount, 
                                        globals, 
                                        input_startingPort, 
                                        exitMutex, 
                                        input_countListeners, 
                                        input_countMaxConnections, 
                                        input_countConnectors));
        serverTP.addTask(tasks[i], true);
    }

    ClientTP.addTask(clientManagerTask, true);
    ClientTP.waitForCompletion();
    serverTP.waitForCompletion();

    TEST_INTRO(testNumber++);

    if ((threadSuccessCount != input_countConnectors))
        TEST_FAILED_MSG(msg + ": served " + TestHelpers::to_string(threadSuccessCount) + "/" + TestHelpers::to_string(input_countConnectors) + " connections");
    else
        TEST_PASSED_MSG(msg);
    pthread_mutex_destroy(&exitMutex);
}

void    MathProtocolMultiServer(
                                        int& exitSignal,  
                                        Globals& globals, 
                                        const int input_startingPort,
                                        pthread_mutex_t& exitMutex,
                                        const int input_countListeners,
                                        const int input_countMaxConnections,
                                        const int input_countConnectors
)
{
    const int countListeners = input_countListeners;
    const int countMaxConnections = input_countMaxConnections;
    const int countConnectors = input_countConnectors;

    ServerContext       ctx;
    Server_MathModule   fakeHttp(Ws::AppLayer::HTTP);
    ctx.setAppLayer(Ws::AppLayer::HTTP, &fakeHttp, &Server_MathModule::InitConnection);

    int maxEvents = countListeners + countMaxConnections;
    int maxFdsEstimate = (countConnectors + countListeners + countMaxConnections) * 1.2f;
    
    Events::Manager eventManager(maxEvents, globals, maxFdsEstimate);
    

    std::vector<Ws::BindInfo> bindAddresses(countListeners);
    prepareBindAddresses(bindAddresses, countListeners, input_startingPort);

    Conn::Manager manager(countMaxConnections, bindAddresses, eventManager, globals, ctx);

    EXPECT_EQUAL(manager.init(), true, "Manager::init() should initialize without issue");

    bool run = true;
    while (run)
    {
        eventManager.ProcessEvents(100);
        pthread_mutex_lock(&exitMutex);
        if (exitSignal != -1)
            run = false;
        pthread_mutex_unlock(&exitMutex);
    }
}
