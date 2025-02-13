

# include "../../DefaultConfig/DefaultConfig.hpp"
# include "../../ServerConfig/ServerConfig.hpp"
# include "../../ServerBlock/ServerBlock.hpp"
# include "../../../GenericUtils/WsTestHelpers/WsTestHelpers.h"
# include <iostream>
# include <arpa/inet.h>
# include <unistd.h>

# include "../../../../Toolkit/TestHelpers/TestHelpers.h"

extern void testPart1(int& testNumber);

int main(void)
{
    int testNumber = 1;

    TEST_HEADER("ServerConfig - Internals");

    testPart1(testNumber);

    TEST_FOOTER;

    return (0);
}
