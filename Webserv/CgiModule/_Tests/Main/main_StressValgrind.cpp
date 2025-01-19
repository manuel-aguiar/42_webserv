
# include <iostream>
# include <unistd.h>
# include "CgiStressTest.hpp"

int main(void)
{
	int testNumber = 1;

	std::cout << "\n*************** CgiModule Stress Tests with Valgrind ***************" << std::endl;

	testNumber = CgiStressTest::StressTest(testNumber, 10, 10, 10, 5000, &CgiStressTest::AllValidCriteria, "AllValidCriteria");
	testNumber = CgiStressTest::StressTest(testNumber, 3, 3, 10, 500, &CgiStressTest::InvalidInterpreterPath, "InvalidInterpreterPath");
	testNumber = CgiStressTest::StressTest(testNumber, 5, 100, 100, 1500, &CgiStressTest::MixedCriteria, "MixedCriteria");	
	testNumber = CgiStressTest::StressTest(testNumber, 5, 50, 100, 2000, &CgiStressTest::MixedCriteria, "MixedCriteria");	
	testNumber = CgiStressTest::StressTest(testNumber, 5, 50, 100, 2000, &CgiStressTest::AllInvalidCriteria, "AllInvalidCriteria");	
	testNumber = CgiStressTest::StressTest(testNumber, 10, 50, 500, 2000, &CgiStressTest::MixedCriteria, "MixedCriteria");	

	close(STDERR_FILENO);

    std::cout << "\n*******************************************************************" << std::endl;
	return (0);
}