
// Project headers
# include "../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../InternalListener/InternalListener.hpp"

// C++ headers
#include <cstdlib>
#include <iostream>

// C headers
#include <arpa/inet.h>

void testInternalListener(int& testNumber)
{
    try
    {
        TEST_INTRO(testNumber++);





        TEST_PASSED_MSG("InternalListener tests passed");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
}