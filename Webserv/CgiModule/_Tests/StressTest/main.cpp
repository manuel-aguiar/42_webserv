

# include <iostream>
# include <unistd.h>
# include "CgiStressTest.hpp"
# include "../../../../Toolkit/TestHelpers/TestHelpers.h"
# include <signal.h>

int main(void)
{
	int testNumber = 1;

	TEST_HEADER("CgiModule Stress Tests");
	::signal(SIGPIPE, SIG_IGN);
	//CgiStressTest::StressTest(testNumber, 5, 5, 5, 5000, &CgiStressTest::ValidPHP, "validPhP", true);
	//CgiStressTest::StressTest(testNumber, 1, 500, 1000, 5000, &CgiStressTest::ValidPHP, "ValidPHP", true);
	CgiStressTest::StressTest(testNumber, 2, 500, 1000, 5000, &CgiStressTest::ValidPHP,"ValidPHP", true);
	//CgiStressTest::StressTest(testNumber, 5, 500, 1000, 5000, &CgiStressTest::AllValidCriteria, "AllValidCriteria");
	//CgiStressTest::StressTest(testNumber, 10, 500, 1000, 5000, &CgiStressTest::AllValidCriteria, "AllValidCriteria");
	//CgiStressTest::StressTest(testNumber, 20, 500, 1000, 5000, &CgiStressTest::ValidPHP, "validPhP", true);
	
	close(STDERR_FILENO);

	TEST_FOOTER;

	return (0);
}