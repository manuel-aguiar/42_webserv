
// Project headers
# include "../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../Manager/Manager.hpp"

// C++ headers
#include <cstdlib>
#include <iostream>

// C headers
#include <arpa/inet.h>

void testManager(int& testNumber)
{
    try
    {
        TEST_INTRO(testNumber++);





        TEST_PASSED_MSG("Manager tests passed");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
}