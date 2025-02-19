
# include <iostream>
# include <unistd.h>
# include "CgiStressTest.hpp"
# include "../../../../Toolkit/TestHelpers/TestHelpers.h"

int main(void)
{
	int testNumber = 1;

	TEST_HEADER("CgiModule Stress Tests with Valgrind");

	CgiStressTest::StressTest(testNumber, 10, 10, 10, 5000, &CgiStressTest::AllValidCriteria, "AllValidCriteria");
	CgiStressTest::StressTest(testNumber, 3, 3, 10, 500, &CgiStressTest::InvalidInterpreterPath, "InvalidInterpreterPath");
	CgiStressTest::StressTest(testNumber, 5, 100, 100, 1500, &CgiStressTest::MixedCriteria, "MixedCriteria");	
	CgiStressTest::StressTest(testNumber, 5, 50, 100, 2000, &CgiStressTest::MixedCriteria, "MixedCriteria");	
	CgiStressTest::StressTest(testNumber, 5, 50, 100, 2000, &CgiStressTest::AllInvalidCriteria, "AllInvalidCriteria");	
	CgiStressTest::StressTest(testNumber, 10, 50, 500, 2000, &CgiStressTest::MixedCriteria, "MixedCriteria");	

	close(STDERR_FILENO);

    TEST_FOOTER;
	return (0);
}