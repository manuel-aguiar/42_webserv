

# include "../../../Toolkit/TestHelpers/TestHelpers.h"

// C++ headers
# include <iostream>

//C headers
# include <unistd.h>
# include <cstdlib>

# include "../Subscription/Subscription.hpp"
# include "../Manager/Manager.hpp"
# include "../../Globals/Globals.hpp"
# include "../InternalSub/InternalSub.hpp"

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);
extern int TestPart3(int testNumber);

int main(void)
{
	int testNumber = 1;

	TEST_HEADER("Events");

	testNumber = TestPart1(testNumber);
	testNumber = TestPart2(testNumber);
	testNumber = TestPart3(testNumber);
	
	::close(STDERR_FILENO);

	TEST_FOOTER;

	return (0);
}

