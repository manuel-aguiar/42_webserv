

# include "../../ServerConfig/ServerConfig.hpp"
# include "../../../../Toolkit/_Tests/test.h"
#include <iostream>
#include <arpa/inet.h>

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

        if (config.getAllBindAddresses().size() != expectedCount)
            throw std::logic_error(
            "result was " + StringUtils::to_string(config.getAllBindAddresses().size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        // checking single server
        const std::vector<ServerBlock>& serverBlocks = config.getServerBlocks();
        if (serverBlocks.size() != 1)
            throw std::logic_error( "there should be only 1 server block"
            "result was " + StringUtils::to_string(serverBlocks.size()) + 
            " but expected: 1" + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


        // checking mapping
        const std::vector<const struct sockaddr*>& sockAddr = serverBlocks[0].getListenSockAddr();
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

        ServerConfig config("OneServer_Wildcard.conf", NULL);
        const int expectedCount = 2;

        config.parseConfigFile();

        if (config.getAllBindAddresses().size() != expectedCount)
            throw std::logic_error(
            "result was " + StringUtils::to_string(config.getAllBindAddresses().size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        // checking single server
        const std::vector<ServerBlock>& serverBlocks = config.getServerBlocks();
        if (serverBlocks.size() != 1)
            throw std::logic_error( "there should be only 1 server block"
            "result was " + StringUtils::to_string(serverBlocks.size()) + 
            " but expected: 1" + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        // checking mapping
        const std::vector<const struct sockaddr*>& sockAddr = serverBlocks[0].getListenSockAddr();
        if (sockAddr.size() != expectedCount)
            throw std::logic_error("sockaddr was not correctly mapped to the server"
            "result was " + StringUtils::to_string(sockAddr.size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED (one server, two listeners(one concrete and one wildcard ip)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

///////////////////////////////////////////////////////////////////////////////////////

    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

        ServerConfig config("OneServer_LocalHost.conf", NULL);
        const int expectedCount = 1;

        config.parseConfigFile();

        if (config.getAllBindAddresses().size() != expectedCount)
            throw std::logic_error(
            "result was " + StringUtils::to_string(config.getAllBindAddresses().size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        // checking single server
        const std::vector<ServerBlock>& serverBlocks = config.getServerBlocks();
        if (serverBlocks.size() != 1)
            throw std::logic_error( "there should be only 1 server block, "
            "result was " + StringUtils::to_string(serverBlocks.size()) + 
            " but expected: 1" + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        // checking mapping
        const std::vector<const struct sockaddr*>& sockAddr = serverBlocks[0].getListenSockAddr();
        if (sockAddr.size() != expectedCount)
            throw std::logic_error("sockaddr was not correctly mapped to the server, "
            "result was " + StringUtils::to_string(sockAddr.size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));



		std::cout << "	PASSED (one server, two equivalent listeners - localhost" << std::endl;
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

        if (config.getAllBindAddresses().size() != expectedCount)
            throw std::logic_error(
            "result was " + StringUtils::to_string(config.getAllBindAddresses().size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        // checking servers
        const std::vector<ServerBlock>& serverBlocks = config.getServerBlocks();
        if (serverBlocks.size() != 2)
            throw std::logic_error( "there should be 2 server blocks"
            "result was " + StringUtils::to_string(serverBlocks.size()) + 
            " but expected: 1" + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        // checking server 0
        const std::vector<const struct sockaddr*>& sockAddr1 = serverBlocks[0].getListenSockAddr();
        if (sockAddr1.size() != expectedCount)
            throw std::logic_error("sockaddr was not correctly mapped to the server"
            "result was " + StringUtils::to_string(sockAddr1.size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        // checking server 1
        const std::vector<const struct sockaddr*>& sockAddr2 = serverBlocks[1].getListenSockAddr();
        if (sockAddr2.size() != expectedCount)
            throw std::logic_error("sockaddr was not correctly mapped to the server"
            "result was " + StringUtils::to_string(sockAddr2.size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED (two servers, same listener)" << std::endl;
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
        //checking config file
        if (config.getAllBindAddresses().size() != expectedCount)
            throw std::logic_error(
            "result was " + StringUtils::to_string(config.getAllBindAddresses().size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


        const std::vector<ServerBlock>& serverBlocks = config.getServerBlocks();

        //checking server 0
        const std::vector<const struct sockaddr*>& sockAddr1 = serverBlocks[0].getListenSockAddr();
        if (sockAddr1.size() != expectedCount)
            throw std::logic_error("sockaddr was not correctly mapped to the server"
            "result was " + StringUtils::to_string(sockAddr1.size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        //checking server 1
        const std::vector<const struct sockaddr*>& sockAddr2 = serverBlocks[1].getListenSockAddr();
        if (sockAddr2.size() != expectedCount)
            throw std::logic_error("sockaddr was not correctly mapped to the server"
            "result was " + StringUtils::to_string(sockAddr2.size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED (two servers, two listeners per server, same (one concrete and one wildcard ip)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
///////////////////////////////////////////////////////////////////////////////////////

    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

        ServerConfig config("TwoServer_LocalHost.conf", NULL);
        const int expectedCount = 1;

        config.parseConfigFile();
        //checking config file
        if (config.getAllBindAddresses().size() != expectedCount)
            throw std::logic_error(
            "result was " + StringUtils::to_string(config.getAllBindAddresses().size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


        const std::vector<ServerBlock>& serverBlocks = config.getServerBlocks();

        //checking server 0
        const std::vector<const struct sockaddr*>& sockAddr1 = serverBlocks[0].getListenSockAddr();
        if (sockAddr1.size() != expectedCount)
            throw std::logic_error("sockaddr was not correctly mapped to the server"
            "result was " + StringUtils::to_string(sockAddr1.size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        //checking server 1
        const std::vector<const struct sockaddr*>& sockAddr2 = serverBlocks[1].getListenSockAddr();
        if (sockAddr2.size() != expectedCount)
            throw std::logic_error("sockaddr was not correctly mapped to the server"
            "result was " + StringUtils::to_string(sockAddr2.size()) + 
            " but expected: " + StringUtils::to_string(expectedCount) + "\n"
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED (two servers, one listener per server, one localhost:80, other 127.0.0.1:80" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
///////////////////////////////////////////////////////////////////////////////////////

std::cout << "\n*************** ******************************************** ***************" << std::endl;

    return (0);
}

