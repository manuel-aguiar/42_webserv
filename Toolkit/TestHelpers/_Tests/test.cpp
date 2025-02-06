

#include "../TestHelpers.h"

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

    TEST_HEADER("Expect Equal");

    try
    {   
        TEST_INTRO(testNumber++);

        Counter counter;
        EXPECT_EQUAL(counter.increase(), 1, "The result must be 1");
        EXPECT_EQUAL(counter.increase(), 2, "The result must be 2");

        TEST_PASSED_MSG("An actual test to make sure the ExpectEqual is not callling the same function twice");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    TEST_FOOTER;

    return (0);
}