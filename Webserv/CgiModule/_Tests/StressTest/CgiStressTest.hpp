
#ifndef CGISTRESSTEST_HPP

# define CGISTRESSTEST_HPP

# include <string>

class TestProtoRequest;

class CgiStressTest
{
    public:
        static void    ValidPython(TestProtoRequest& proto, int index = 0);
        static void    ValidShell(TestProtoRequest& proto, int index = 0);
        static void    ValidPHP(TestProtoRequest& proto, int index = 0);
        static void    InvalidScript(TestProtoRequest& proto, int index = 0);
        static void    InvalidInterpreter(TestProtoRequest& proto, int index = 0);
        static void    InvalidInterpreterPath(TestProtoRequest& proto, int index = 0);

        // Assignment Criteria, between scripts, bad interpreters, a bit of everything
        static void    MixedCriteria        (TestProtoRequest& proto, int index);
        static void    AllValidCriteria   (TestProtoRequest& proto, int index);
        static void    AllInvalidCriteria   (TestProtoRequest& proto, int index);
        
        static std::string 
                        ValgrindReadandClear(const char* filename);
        static void     StressTest(int& testNumber, 
                                const int workers, 
                                const int backlog,
                                const int workerBufferSize, 
                                const int connectionCount, 
                                const unsigned int timeoutMs,
                                void (*AssignmentCriteria)(TestProtoRequest& proto, int index),
                                const char* assignmentDescription,
                                const bool isDebugPrint = false);
        
        static void prepareExpectedOutput(bool isExpectedValid, TestProtoRequest& proto);
};



#endif