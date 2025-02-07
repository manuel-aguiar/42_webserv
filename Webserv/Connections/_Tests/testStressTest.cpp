
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

void    StressTestManager_MathProtocol(  
                                        int& testNumber,
                                        ThreadPoolGeneric& tp, 
                                        Globals& globals, 
                                        const int startingPort,
                                        pthread_mutex_t& writeMutex,
                                        const int input_countListeners,
                                        const int input_countMaxConnections,
                                        const int input_countConnectors,
                                        const int input_clientTimeoutMs
);

struct StressTestTask : public IThreadTask
{
    int& testNumber;
    ThreadPoolGeneric& tp;
    Globals& globals;
    const int startingPort;
    pthread_mutex_t& writeMutex;
    const int input_countListeners;
    const int input_countMaxConnections;
    const int input_countConnectors;
    const int input_clientTimeoutMs;
    
    StressTestTask(
        int& testNumber,
        ThreadPoolGeneric& tp, 
        Globals& globals, 
        const int startingPort,
        pthread_mutex_t& writeMutex,
        const int input_countListeners,
        const int input_countMaxConnections,
        const int input_countConnectors,
        const int input_clientTimeoutMs
    ) : 
        testNumber(testNumber),
        tp(tp),
        globals(globals),
        startingPort(startingPort),
        writeMutex(writeMutex),
        input_countListeners(input_countListeners),
        input_countMaxConnections(input_countMaxConnections),
        input_countConnectors(input_countConnectors),
        input_clientTimeoutMs(input_clientTimeoutMs)
    {}

    void execute()
    {
        StressTestManager_MathProtocol(testNumber, tp, globals, startingPort, writeMutex, input_countListeners, input_countMaxConnections, input_countConnectors, input_clientTimeoutMs);
    }
};

void testStressTest(int& testNumber)
{
    ThreadPoolHeap      tp(1, 11);
    ThreadPoolHeap      clientTp(1,50);
    Globals             globals(NULL, NULL, NULL, NULL);
    pthread_mutex_t     writeMutex;

    pthread_mutex_init(&writeMutex, NULL);
    //Stress tests
    std::cout << TEST_CLR_BROWN << "\n\tStress tests (failure is not an error)" << TEST_CLR_RESET << std::endl;
    StressTestTask task1(testNumber,  clientTp , globals, 7080, writeMutex, 1, 10, 100, 5000);
    StressTestTask task2(testNumber,  clientTp , globals, 7180, writeMutex, 10, 10, 100, 5000);
    StressTestTask task3(testNumber,  clientTp , globals, 7280, writeMutex, 10, 100, 1000, 1000);
    StressTestTask task4(testNumber,  clientTp , globals, 7380, writeMutex, 10, 100, 10000, 2000);
    StressTestTask task5(testNumber,  clientTp , globals, 7480, writeMutex, 10, 100, 10000, 5000);
    StressTestTask task6(testNumber,  clientTp , globals, 7580, writeMutex, 100, 100, 10000, 2000);
    StressTestTask task7(testNumber,  clientTp , globals, 7680, writeMutex, 100, 1000, 10000, 3000);
    StressTestTask task8(testNumber,  clientTp , globals, 7780, writeMutex, 10, 5000, 10000, 3000);
    StressTestTask task9(testNumber,  clientTp , globals, 7880, writeMutex, 1, 5000, 10000, 5000);
    StressTestTask task10(testNumber, clientTp , globals, 7980, writeMutex, 1, 2000, 10000, 5000);
    StressTestTask task11(testNumber, clientTp , globals, 8080, writeMutex, 1, 1000, 10000, 5000); 
    
    tp.addTask(task1, true);
    tp.addTask(task2, true);
    tp.addTask(task3, true);
    tp.addTask(task4, true);
    tp.addTask(task5, true);
    tp.addTask(task6, true);
    tp.addTask(task7, true);
    tp.addTask(task8, true);
    tp.addTask(task9, true);
    tp.addTask(task10, true);
    tp.addTask(task11, true);

    tp.waitForCompletion();

    pthread_mutex_destroy(&writeMutex);  
}



void testStressTest_Linear(int& testNumber)
{
    ThreadPoolHeap      clientTp(50,50);
    Globals             globals(NULL, NULL, NULL, NULL);
    pthread_mutex_t     writeMutex;

    pthread_mutex_init(&writeMutex, NULL);
    //Stress tests
    std::cout << TEST_CLR_BROWN << "\n\tStress tests (failure is not an error)" << TEST_CLR_RESET << std::endl;
    StressTestManager_MathProtocol(testNumber,  clientTp , globals, 7080, writeMutex, 1, 10, 100, 5000);
    StressTestManager_MathProtocol(testNumber,  clientTp , globals, 7180, writeMutex, 10, 10, 100, 5000);
    StressTestManager_MathProtocol(testNumber,  clientTp , globals, 7280, writeMutex, 10, 100, 1000, 1000);
    StressTestManager_MathProtocol(testNumber,  clientTp , globals, 7380, writeMutex, 10, 100, 10000, 2000);
    StressTestManager_MathProtocol(testNumber,  clientTp , globals, 7480, writeMutex, 10, 100, 10000, 5000);
    StressTestManager_MathProtocol(testNumber,  clientTp , globals, 7580, writeMutex, 100, 100, 10000, 2000);
    StressTestManager_MathProtocol(testNumber,  clientTp , globals, 7680, writeMutex, 100, 1000, 10000, 3000);
    StressTestManager_MathProtocol(testNumber,  clientTp , globals, 7780, writeMutex, 10, 5000, 10000, 3000);
    StressTestManager_MathProtocol(testNumber,  clientTp , globals, 7880, writeMutex, 1, 5000, 10000, 5000);
    StressTestManager_MathProtocol(testNumber, clientTp , globals, 7980, writeMutex, 1, 2000, 10000, 5000);
    StressTestManager_MathProtocol(testNumber, clientTp , globals, 8080, writeMutex, 1, 1000, 10000, 5000); 
    
    pthread_mutex_destroy(&writeMutex);  
}

void    StressTestManager_MathProtocol(  
                                        int& testNumber,
                                        ThreadPoolGeneric& tp, 
                                        Globals& globals, 
                                        const int startingPort,
                                        pthread_mutex_t& writeMutex,
                                        const int input_countListeners,
                                        const int input_countMaxConnections,
                                        const int input_countConnectors,
                                        const int input_clientTimeoutMs
)
{
    const int countListeners = input_countListeners;
    const int countMaxConnections = input_countMaxConnections;
    const int countConnectors = input_countConnectors;
    const int clientTimeoutMs = input_clientTimeoutMs;

    ServerContext       ctx;
    Server_MathModule   fakeHttp(Ws::AppLayer::HTTP);
    ctx.setAppLayer(Ws::AppLayer::HTTP, &fakeHttp, &Server_MathModule::InitConnection);

    std::string msg = "Manager: StressTest, MathProtocol: " 
                    + TestHelpers::to_string(countListeners) + " listeners, "
                    + TestHelpers::to_string(countMaxConnections) + " max connections, "
                    + TestHelpers::to_string(countConnectors) + " connectors, "
                    + TestHelpers::to_string(clientTimeoutMs) + "ms timeout";

    int maxEvents = countListeners + countMaxConnections;
    int maxFdsEstimate = (countConnectors + countListeners + countMaxConnections) * 1.2f;
    
    Events::Manager eventManager(maxEvents, globals, maxFdsEstimate);
    

    std::vector<Ws::BindInfo> bindAddresses(countListeners);
    prepareBindAddresses(bindAddresses, countListeners, startingPort);

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
                                                    clientTimeoutMs,
                                                    startingPort);
    tp.addTask(clientManagerTask, true);

    bool run = true;
    while (run)
    {
        eventManager.ProcessEvents(100);
            pthread_mutex_lock(&mutex);
            if (threadSuccessCount != -1)
                run = false;
            pthread_mutex_unlock(&mutex);
    }
    
    pthread_mutex_destroy(&mutex);

    manager.shutdown();

    pthread_mutex_lock(&writeMutex);
    TEST_INTRO(testNumber++);

    if ((fakeHttp.serveCount != countConnectors) || (threadSuccessCount != countConnectors))
        TEST_FAILED_MSG(msg + ": served " + TestHelpers::to_string(threadSuccessCount) + "/" + TestHelpers::to_string(countConnectors) + " connections");
    else
        TEST_PASSED_MSG(msg);
    pthread_mutex_unlock(&writeMutex);
}
