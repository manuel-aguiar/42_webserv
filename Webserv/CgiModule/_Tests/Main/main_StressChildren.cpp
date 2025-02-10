

# include <iostream>
# include "CgiStressTest.hpp"
# include <unistd.h>
# include "../../../../Toolkit/TestHelpers/TestHelpers.h"

extern void TestPart1(int& testNumber);
extern void TestPart2(int& testNumber);

int main(void)
{
	int testNumber = 1;

    TEST_HEADER("CgiModule Stress Tests, trace-children valgrind");

    TestPart1(testNumber);
    TestPart2(testNumber);
	CgiStressTest::StressTest(testNumber, 5, 50, 100, 5000, &CgiStressTest::AllInvalidCriteria, "AllInvalidCriteria");	
    CgiStressTest::StressTest(testNumber, 3, 10, 100, 5000, &CgiStressTest::AllValidCriteria, "AllValidCriteria");	
    CgiStressTest::StressTest(testNumber, 5, 50, 100, 5000, &CgiStressTest::AllValidCriteria, "AllValidCriteria");	

    close(STDERR_FILENO);

    TEST_FOOTER;

	return (0);
}