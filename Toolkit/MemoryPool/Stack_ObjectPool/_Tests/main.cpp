

#include "../Stack_ObjectPool.hpp"
#include <list>

#include "../../../TestHelpers/TestHelpers.h"


int main(void)
{
    //int testNumber = 1;

	TEST_HEADER("Stack_ObjectPool");

    Stack_ObjectPool<int, 10>     pool1;
    
	TEST_FOOTER;
	
    return 0;
}