
// Project headers
# include "../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../Accepter/Accepter.hpp"

// C++ headers
#include <cstdlib>
#include <iostream>

// C headers
#include <arpa/inet.h>

void testAccepter(int& testNumber)
{
    try
    {
        TEST_INTRO(testNumber++);





        TEST_PASSED_MSG("Accepter tests passed");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
}