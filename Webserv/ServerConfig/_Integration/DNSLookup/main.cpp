

# include "../../ServerConfig/ServerConfig.hpp"
# include "../../../../Toolkit/_Tests/test.h"
#include <iostream>


int main(void)
{

	std::cout << "\n*************** [ServerConfig - DNS Lookup] Integration test ***************\n" << std::endl;

    int testNumber = 1;
///////////////////////////////////////////////////////////////////////////////////////
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

        ServerConfig config("OneServer_TwoListen.conf", NULL);
        const int expectedCount = 2;

        config.parseConfigFile();

        if (config.getAllSockaddr().size() != expectedCount)
            throw std::logic_error(
            "result was " + StringUtils::to_string(config.getAllSockaddr().size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        const std::vector<ServerBlock>& serverBlocks = config.getServerBlocks();
        if (serverBlocks.size() != 1)
            throw std::logic_error( "there should be only 1 server block"
            "result was " + StringUtils::to_string(serverBlocks.size()) + 
            " but expected: 1" + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        const std::vector<const struct sockaddr*> sockAddr = serverBlocks[0].getSockAddr();

        if (sockAddr.size() != expectedCount)
            throw std::logic_error("sockaddr was not correctly mapped to the server"
            "result was " + StringUtils::to_string(sockAddr.size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED (one server, two listeners)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
///////////////////////////////////////////////////////////////////////////////////////
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

        ServerConfig config("TwoServer_SameListen.conf", NULL);
        const int expectedCount = 1;

        config.parseConfigFile();

        if (config.getAllSockaddr().size() != expectedCount)
            throw std::logic_error(
            "result was " + StringUtils::to_string(config.getAllSockaddr().size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


		std::cout << "	PASSED (two servers, same listener)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}    
///////////////////////////////////////////////////////////////////////////////////////
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

        ServerConfig config("OneServer_Wildcard.conf", NULL);
        const int expectedCount = 2;

        config.parseConfigFile();

        if (config.getAllSockaddr().size() != expectedCount)
            throw std::logic_error(
            "result was " + StringUtils::to_string(config.getAllSockaddr().size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


		std::cout << "	PASSED (one server, two listeners(one concrete and one wildcard ip)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

        ServerConfig config("TwoServer_Wildcard.conf", NULL);
        const int expectedCount = 2;

        config.parseConfigFile();

        if (config.getAllSockaddr().size() != expectedCount)
            throw std::logic_error(
            "result was " + StringUtils::to_string(config.getAllSockaddr().size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


		std::cout << "	PASSED (two servers, two listeners per server, same (one concrete and one wildcard ip)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
///////////////////////////////////////////////////////////////////////////////////////

std::cout << "\n*************** ******************************************** ***************" << std::endl;

    return (0);
}

