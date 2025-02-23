

# include "../../../../Toolkit/TestHelpers/TestHelpers.h"

extern void testFullRead(int& testNumber);
extern void testPartialRead(int& testNumber);

int main(void)
{
	int testNumber = 1;

	TEST_HEADER("Cgi Output Parsing");
	
	testFullRead(testNumber);
	testPartialRead(testNumber);

	TEST_FOOTER;

	return (0);
}