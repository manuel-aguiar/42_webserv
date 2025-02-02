
// Project headers
# include "../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../ListeningSocket/ListeningSocket.hpp"

// C++ headers
#include <cstdlib>
#include <iostream>

// C headers
#include <arpa/inet.h>

void testListeningSocket(int& testNumber)
{
    try
    {
        TEST_INTRO(testNumber++);





        TEST_PASSED_MSG("ListeningSocket tests passed");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
}