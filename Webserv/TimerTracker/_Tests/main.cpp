
# include "../../../Toolkit/TestHelpers/TestHelpers.h"
# include <iostream>

extern void testTimer(int& testNumber);
extern void testTimerTracker1(int& testNumber);
extern void testTimerTracker2(int& testNumber);

int main(void)
{	
    int testNumber = 1;
    
    TEST_HEADER("TimerTracker");
	
    testTimer(testNumber);
    testTimerTracker1(testNumber);
    testTimerTracker2(testNumber);
    
    TEST_FOOTER;

    return (0);
}