


#include "../../../Toolkit/TestHelpers/TestHelpers.h"
#include "../Timer/Timer.hpp"
#include <iostream>


void testTimer(int& testNumber)
{

    try
    {   
        TEST_INTRO(testNumber++);

        Timer first = Timer::now();
        Timer second = Timer::now() + 1;
        EXPECT_EQUAL(second > first, true, "second timer should appear later than the first timer");

        first.update();
        first += 1;
        EXPECT_EQUAL(first > second, true, "after update, first should be later than second");

        TEST_PASSED_MSG("Timer tests passed");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
}