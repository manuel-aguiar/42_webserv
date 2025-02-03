

#include "../BlockFinder.hpp"
#include <iomanip>
#include <arpa/inet.h>
#include <cstdlib>

#include "../../../../Toolkit/TestHelpers/TestHelpers.h"

extern void reviewTests();

int main(void)
{
	TEST_HEADER("BlockFinder");

	reviewTests();

	TEST_FOOTER;
	return (0);
}