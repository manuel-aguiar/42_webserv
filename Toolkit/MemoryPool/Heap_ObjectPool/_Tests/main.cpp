
#include "../Heap_ObjectPool.hpp"
#include <list>

#include "../../../TestHelpers/TestHelpers.h"

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);

int main(void)
{
    int testNumber = 1;

    TEST_HEADER("Heap_ObjectPool");
	
    testNumber = TestPart1(testNumber);
    testNumber = TestPart2(testNumber);

    TEST_FOOTER;

    return 0;
}