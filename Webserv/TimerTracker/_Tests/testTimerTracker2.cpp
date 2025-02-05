

# include "../TimerTracker.hpp"
# include "../Timer/Timer.hpp"
# include "../../../Toolkit/TestHelpers/TestHelpers.h"

struct SomeStruct
{
    int a;

    bool operator==(const SomeStruct& other) const
    {
        return (a == other.a);
    }

    bool operator!=(const SomeStruct& other) const
    {
        return !(*this == other);
    }
};

void testTimerTracker2(int& testNumber)
{
    try
    {
        TEST_INTRO(testNumber++);

        TimerTracker<Timer,  SomeStruct> tracker(4);

        tracker.insert(Timer::now(), (SomeStruct){ .a = 1});
        tracker.insert(Timer::now(), (SomeStruct){ .a = 2});

        Timer cuttoff = Timer::now() + 1;
        
        tracker.insert(Timer::now() + 2 , (SomeStruct){ .a = 3});
        tracker.insert(Timer::now() + 3, (SomeStruct){ .a = 4});

        //tracker.insert(Timer::now(), (SomeStruct){ .a = 5}); <- correctly asserts, room for only 4 timers

        // cleaning old timers
        for (TimerTracker<Timer, SomeStruct>::iterator it = tracker.begin(); it != tracker.end(); ++it)
        {
            if (it->first < cuttoff)
                tracker.erase(it);
            else
                break;
        }

        EXPECT_EQUAL(tracker.size(), 2, "erased 2 elements, left with other 2");

		TEST_PASSED_MSG("TimerTracker with actual timer");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    try
    {
        TEST_INTRO(testNumber++);

        TimerTracker<Timer,  SomeStruct> tracker(4);

        tracker.insert(Timer::now(), (SomeStruct){ .a = 1});
        tracker.insert(Timer::now(), (SomeStruct){ .a = 2});

        Timer target = Timer::now() + 10;
        
        tracker.insert(target, (SomeStruct){ .a = 3});
        tracker.insert(Timer::now(), (SomeStruct){ .a = 4});

        //tracker.insert(Timer::now(), (SomeStruct){ .a = 5}); <- correctly asserts, room for only 4 timers

        tracker.erase(target, (SomeStruct){ .a = 4});
        EXPECT_EQUAL(tracker.size(), 4, "no timers removed, no match");
        
        tracker.erase(target, (SomeStruct){ .a = 3});
        EXPECT_EQUAL(tracker.size(), 3, "erased 1 elements, left with other 3");

		TEST_PASSED_MSG("TimerTracker erasing specific timer");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

}