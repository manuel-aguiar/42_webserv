


# include "../../../Toolkit/TestHelpers/TestHelpers.h"

extern void testMonitor(int& testNumber);
extern void testManager(int& testNumber);
extern void testInternalListener(int& testNumber);
extern void testConnection(int& testNumber);
extern void testListener(int& testNumber);
extern void testStressTest(int& testNumber);
extern void testMultiServer(int& testNumber);
extern void testValgrindStress(int& testNumber);

int main(void)
{
	int testNumber = 1;

	TEST_HEADER("Connection Manager tests");

	testMonitor			(testNumber);
	testInternalListener	(testNumber);
	testConnection		(testNumber);
	testListener		(testNumber);
	testManager			(testNumber);
	
	TEST_FOOTER;

	return (0);
}