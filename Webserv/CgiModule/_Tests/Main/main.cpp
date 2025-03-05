

# include <iostream>
# include <unistd.h>
# include "../../../../Toolkit/TestHelpers/TestHelpers.h"

extern void TestPart1(int& testNumber);
extern void TestPart2(int& testNumber);

int main(void)
{
	int testNumber = 1;
	TEST_HEADER("CgiModule Functional Tests");

	//TestPart1(testNumber);
	TestPart2(testNumber);

	close(STDERR_FILENO);

	TEST_FOOTER;

	return (0);
}
