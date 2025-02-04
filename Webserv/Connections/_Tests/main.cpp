


# include "../../../Toolkit/TestHelpers/TestHelpers.h"

extern void testSocket(int& testNumber);
extern void testMonitor(int& testNumber);
extern void testManager(int& testNumber);
extern void testListeningSocket(int& testNumber);
extern void testConnection(int& testNumber);
extern void testAccepter(int& testNumber);
extern void testAppLayer(int& testNumber);

int main(void)
{
	int testNumber = 1;

	TEST_HEADER("Connection Manager tests");

	//testSocket			(testNumber);
	//testMonitor			(testNumber);
	//testListeningSocket	(testNumber);
	//testConnection		(testNumber);
	//testAccepter		(testNumber);
	//testAppLayer		(testNumber);
	testManager			(testNumber);

	TEST_FOOTER;

	return (0);
}