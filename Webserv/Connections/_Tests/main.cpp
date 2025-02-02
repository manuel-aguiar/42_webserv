


# include "../../../Toolkit/TestHelpers/TestHelpers.h"

extern void testSocket(int& testNumber);

int main(void)
{
	int testNumber = 1;

	TEST_HEADER("Connection Manager tests");

	testSocket(testNumber);	

	TEST_FOOTER;

	return (0);
}