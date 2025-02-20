

# include "../../../../Toolkit/TestHelpers/TestHelpers.h"

extern void testFullRead(int& testNumber);

int main(void)
{
	int testNumber = 1;

	TEST_HEADER("Cgi Output Parsing");
	
	testFullRead(testNumber);

	TEST_FOOTER;

	return (0);
}