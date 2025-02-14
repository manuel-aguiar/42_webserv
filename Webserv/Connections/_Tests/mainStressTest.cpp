


# include "../../../Toolkit/TestHelpers/TestHelpers.h"

extern void testStressTest(int& testNumber);

int main(void)
{
	int testNumber = 1;

	TEST_HEADER("Connection Manager Stress tests");

	testStressTest		(testNumber);
	
	TEST_FOOTER;

	return (0);
}