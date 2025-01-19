

# include <iostream>
# include "CgiStressTest.hpp"
# include <unistd.h>

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);

int main(void)
{
	int testNumber = 1;

	std::cout << "\n*************** CgiModule Stress Children with Valgrind ***************" << std::endl;

    testNumber = TestPart1(testNumber);
    testNumber = TestPart2(testNumber);
    testNumber = CgiStressTest::StressTest(testNumber, 5, 50, 100, 5000, &CgiStressTest::InvalidInterpreterPath, "InvalidInterpreterPath");	
    testNumber = CgiStressTest::StressTest(testNumber, 5, 50, 100, 5000, &CgiStressTest::InvalidInterpreter, "InvalidInterpreter");	
    //testNumber = CgiStressTest::StressTest(testNumber, 5, 50, 100, 5000, &CgiStressTest::InvalidScript, "InvalidScript");
	testNumber = CgiStressTest::StressTest(testNumber, 5, 50, 100, 5000, &CgiStressTest::AllInvalidCriteria, "AllInvalidCriteria");	
    testNumber = CgiStressTest::StressTest(testNumber, 3, 10, 100, 5000, &CgiStressTest::MixedCriteria, "MixedCriteria");	
    testNumber = CgiStressTest::StressTest(testNumber, 5, 50, 100, 5000, &CgiStressTest::AllValidCriteria, "AllValidCriteria");	

    close(STDERR_FILENO);

    std::cout << "\n*******************************************************************" << std::endl;
	return (0);
}