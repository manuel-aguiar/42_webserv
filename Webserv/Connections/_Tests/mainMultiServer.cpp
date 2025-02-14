


# include "../../../Toolkit/TestHelpers/TestHelpers.h"

extern void testMultiServer(int& testNumber);

int main(void)
{
	int testNumber = 1;

	TEST_HEADER("Connection Manager MultiServer");

	testMultiServer		(testNumber);
	
	TEST_FOOTER;

	return (0);
}