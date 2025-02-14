

# include "../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../../../Toolkit/ThreadPool/ThreadPool.hpp"
# include "../../Globals/Globals.hpp"
# include "../../ServerContext/ServerContext.hpp"

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

void testValgrindStress(int& testNumber)
{
    // global setup for all tests
    ThreadPool<20, 50>  serverTp;
    ThreadPoolHeap      clientTp(2, 20);
    Globals globals(NULL, NULL, NULL, NULL);
    ServerContext ctx;
    pthread_mutex_t     writeMutex;

//////////////////////////////////////////////////////
    //Stress tests
    pthread_mutex_init(&writeMutex, NULL);
    std::cout << TEST_CLR_BROWN << "\n\tStress tests (failure is not an error)" << TEST_CLR_RESET << std::endl;
    StressTestManager_MathProtocol(testNumber,  serverTp , globals, 7080, writeMutex, 1, 10, 100, 5000);
    StressTestManager_MathProtocol(testNumber,  serverTp , globals, 7280, writeMutex, 10, 100, 1000, 1000);

    pthread_mutex_destroy(&writeMutex);

//////////////////////////////////////////////////////

    //multiserver
    std::cout << TEST_CLR_BROWN << "\n\tMultiServer Stress tests (failure is not an error)" << TEST_CLR_RESET << std::endl;
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals,  1, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals,  6, 8080, 10, 100, 10000, 500);
    StressTest_MultiServer(testNumber, serverTp, clientTp, globals, 16, 8080, 10, 100, 10000, 500);
}