

#include "../TestHelpers.h"
#include <iostream>

extern size_t mystrlen(const char* str);

struct Counter
{    
    Counter() : counter (0) {}
    int counter;
    int increase()
    {
        ++counter;
        return (counter);
    }
};

int main(void)
{
    int testNumber = 1;

    TEST_HEADER("Demo");

    try
    {   
        TEST_INTRO(testNumber++);

        Counter counter;
        EXPECT_EQUAL(counter.increase(), 1, "The result must be 1");
        EXPECT_EQUAL(counter.increase(), 2, "The result must be 1");

        TEST_PASSED_MSG("An actual test to make sure the ExpectEqual is not callling the same function twice");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    try
    {   
        TEST_INTRO(testNumber++);

        EXPECT_EQUAL(2 + 3 , 2 + 3, "The result must be 5");

        TEST_PASSED_MSG("if two sums are equal, for example");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
///////////////////////////////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);

        EXPECT_EQUAL(1 + 3 , 2 + 3, "The result must be 5");

        TEST_PASSED_MSG("comparing results");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
//////////////////////////////////////////////////////////////////////
    try
    {   
        TEST_INTRO(testNumber++);

        EXPECT_EQUAL(std::string("Hello World!"), std::string("Bye World!"), "String must be equal");

        TEST_PASSED_MSG("Some string tests");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }    

//////////////////////////////////////////////////////////////////////
    try
    {   
        TEST_INTRO(testNumber++);

        EXPECT_EQUAL(mystrlen("Hello World!"), 12, "Strlen of Hello World! should be 12");

        TEST_PASSED_MSG("checking strlen of a string");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }  

    TEST_FOOTER;

    return (0);
}