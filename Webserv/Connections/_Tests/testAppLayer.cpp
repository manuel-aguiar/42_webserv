
// Project headers
# include "../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../AppLayer/AppLayer.hpp"

// C++ headers
#include <cstdlib>
#include <iostream>

// C headers
#include <arpa/inet.h>

void testAppLayer(int& testNumber)
{
    try
    {
        TEST_INTRO(testNumber++);

        //There is really nothing to test here, honestly



        TEST_PASSED_MSG("AppLayer tests passed");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
}