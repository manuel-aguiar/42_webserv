

#include "../TestHelpers.h"
#include <iostream>
#include <cstring>

int main(void)
{
    int testNumber = 1;

    TEST_HEADER("Demo");

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

        EXPECT_EQUAL(std::strlen("Hello World!"), 12, "Strlen of Hello World! should be 12");

        TEST_PASSED_MSG("checking strlen of a string");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }  

    TEST_FOOTER;

    return (0);
}