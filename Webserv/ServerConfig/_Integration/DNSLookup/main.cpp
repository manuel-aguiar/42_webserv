

# include "../../ServerConfig/ServerConfig.hpp"
# include "../../../../Toolkit/_Tests/test.h"
#include <iostream>


int main(void)
{
    int testNumber = 1;

    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

        ServerConfig config("TwoListen.conf", NULL);
        const int expectedCount = 2;

        config.parseConfigFile();

        if (config.getAllSockaddr().size() != expectedCount)
            throw std::logic_error(
            "result was " + StringUtils::to_string(config.getAllSockaddr().size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
    


    return (0);
}

