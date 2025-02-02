

# include "../TimerTracker.hpp"
# include "../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../../../Toolkit/MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.hpp"
# include "../../../Toolkit/MemoryPool/Nginx_PoolAllocator/Nginx_PoolAllocator.hpp"

void testTimerTracker1(int& testNumber)
{
    try
    {
        TEST_INTRO(testNumber++);

        TimerTracker<int, int> tracker(123);
		TEST_PASSED_MSG("TimerTracker instantiation");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
//////////////////////////////////////////////////////////////////////////////////// 
    try
    {   //with a pool
        TEST_INTRO(testNumber++);
        
        Nginx_MemoryPool pool(20000, 1);

        //some BS, will just rebind for internal arrays
        Nginx_PoolAllocator<int> alloc(pool);

        TimerTracker<int, int, Nginx_PoolAllocator<int> > tracker(123, alloc);
 
		TEST_PASSED_MSG("TimerTracker allocating inside a memory pool");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
//////////////////////////////////////////////////////////////////////////////////// 
}