
# include "../../DefaultConfig/DefaultConfig.hpp"
# include "../../ServerConfig/ServerConfig.hpp"
# include "../../ServerBlock/ServerBlock.hpp"
# include "../../../GenericUtils/WsTestHelpers/WsTestHelpers.h"
# include <iostream>
# include <arpa/inet.h>
# include <unistd.h>


int main(void)
{
    TEST_HEADER("Integration ServerConfig - DNS Lookup");

    int testNumber = 1;
///////////////////////////////////////////////////////////////////////////////////////

    DefaultConfig defaultConfig;

    try
    {
        TEST_INTRO(testNumber++);

        ServerConfig config("OneServer_TwoListen.conf", defaultConfig);
        const size_t expectedCount = 2;

        config.parseConfigFile();

        EXPECT_EQUAL(config.getAllBindAddresses().size(), expectedCount, "Wrong number of addresses");

        // checking single server
        const std::vector<ServerBlock>& serverBlocks = config.getServerBlocks();

        EXPECT_EQUAL(serverBlocks.size(), 1, "there should be only 1 server block");

        // checking mapping
        const std::vector<const Ws::Sock::addr*>& sockAddr = serverBlocks[0].getListenAddresses();

        EXPECT_EQUAL(sockAddr.size(), expectedCount, "sockaddr was not correctly mapped to the server");
        
         //confirming match is correct ,ORDER DEPENDS ON SETS
        Ws::Sock::addr_in *addr;
        char ip[INET_ADDRSTRLEN];


        // checking first address
        addr = (Ws::Sock::addr_in*)(sockAddr[0]);
        ::inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN);

        EXPECT_EQUAL(::ntohs(addr->sin_port), (uint16_t)80, "sockaddr was not correctly mapped to the server");
        EXPECT_EQUAL(std::string(ip), std::string("0.0.0.0"), "sockaddr was not correctly mapped to the server");
        
        //checking second address
        addr = (Ws::Sock::addr_in*)(sockAddr[1]);
        ::inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN);

        EXPECT_EQUAL(::ntohs(addr->sin_port), (uint16_t)81, "sockaddr was not correctly mapped to the server");
        EXPECT_EQUAL(std::string(ip), std::string("0.0.0.0"), "sockaddr was not correctly mapped to the server");

        TEST_PASSED_MSG("one server, two listeners");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

///////////////////////////////////////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);

        ServerConfig config("OneServer_Wildcard.conf", defaultConfig);
        const size_t expectedCount = 2;

        config.parseConfigFile();

        EXPECT_EQUAL(config.getAllBindAddresses().size(), expectedCount, "Wrong number of addresses");
        
        // checking single server
        const std::vector<ServerBlock>& serverBlocks = config.getServerBlocks();
        EXPECT_EQUAL(serverBlocks.size(), 1, "there should be only 1 server block");

        // checking mapping
        const std::vector<const Ws::Sock::addr*>& sockAddr = serverBlocks[0].getListenAddresses();
        EXPECT_EQUAL(sockAddr.size(), expectedCount, "sockaddr was not correctly mapped to the server");

         //confirming match is correct ,ORDER DEPENDS ON SETS
        char ip[INET_ADDRSTRLEN];
        Ws::Sock::addr_in *addr;

        addr = (Ws::Sock::addr_in*)(sockAddr[0]);
        ::inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN);

        EXPECT_EQUAL(::ntohs(addr->sin_port), 80, "sockaddr was not correctly mapped to the server");
        EXPECT_EQUAL(std::string(ip), std::string("0.0.0.0"), "sockaddr was not correctly mapped to the server");

        addr = (Ws::Sock::addr_in*)(sockAddr[1]);
        ::inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN);

        EXPECT_EQUAL(::ntohs(addr->sin_port), 80, "sockaddr was not correctly mapped to the server");
        EXPECT_EQUAL(std::string(ip), std::string("123.123.123.123"), "sockaddr was not correctly mapped to the server");

        TEST_PASSED_MSG("one server, two listeners(one concrete and one wildcard ip");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

///////////////////////////////////////////////////////////////////////////////////////

    try
    {
        TEST_INTRO(testNumber++);

        ServerConfig config("OneServer_LocalHost.conf", defaultConfig);
        const size_t expectedCount = 1;

        config.parseConfigFile();

        EXPECT_EQUAL(config.getAllBindAddresses().size(), expectedCount, "Wrong number of addresses");
        
        // checking single server
        const std::vector<ServerBlock>& serverBlocks = config.getServerBlocks();

        EXPECT_EQUAL(serverBlocks.size(), 1, "there should be only 1 server block");

        // checking mapping
        const std::vector<const Ws::Sock::addr*>& sockAddr = serverBlocks[0].getListenAddresses();

        EXPECT_EQUAL(sockAddr.size(), expectedCount, "sockaddr was not correctly mapped to the server");

        //confirming match is correct ,ORDER DEPENDS ON SETS
        char ip[INET_ADDRSTRLEN];
        Ws::Sock::addr_in *addr;
        
        addr = (Ws::Sock::addr_in*)(sockAddr[0]);
        ::inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN);

        EXPECT_EQUAL(::ntohs(addr->sin_port), 80, "sockaddr was not correctly mapped to the server");
        EXPECT_EQUAL(std::string(ip), std::string("127.0.0.1"), "sockaddr was not correctly mapped to the server");

        TEST_PASSED_MSG("one server, two equivalent listeners - localhost");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

///////////////////////////////////////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);

        ServerConfig config("TwoServer_SameListen.conf", defaultConfig);
        const size_t expectedCount = 1;

        config.parseConfigFile();

        EXPECT_EQUAL(config.getAllBindAddresses().size(), expectedCount, "Wrong number of addresses");

        const std::vector<ServerBlock>& serverBlocks = config.getServerBlocks();

        EXPECT_EQUAL(serverBlocks.size(), 2, "there should be 2 server blocks");

        const std::vector<const Ws::Sock::addr*>& sockAddr1 = serverBlocks[0].getListenAddresses();
        const std::vector<const Ws::Sock::addr*>& sockAddr2 = serverBlocks[1].getListenAddresses();

        EXPECT_EQUAL(sockAddr1.size(), expectedCount, "sockaddr was not correctly mapped to server 1");
        EXPECT_EQUAL(sockAddr2.size(), expectedCount, "sockaddr was not correctly mapped to server 2");

        EXPECT_EQUAL(sockAddr1[0], sockAddr2[0], "both servers should be pointing to the same sockaddr");

        char ip[INET_ADDRSTRLEN];
        Ws::Sock::addr_in* addr = (Ws::Sock::addr_in*)(sockAddr1[0]);
        ::inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN);

        EXPECT_EQUAL(::ntohs(addr->sin_port), (uint16_t)80, "Port mapping failed");
        EXPECT_EQUAL(std::string(ip), std::string("0.0.0.0"), "IP mapping failed");

        TEST_PASSED_MSG("two servers, same listener");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

///////////////////////////////////////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);

        ServerConfig config("TwoServer_Wildcard.conf", defaultConfig);
        const size_t expectedCount = 2;

        config.parseConfigFile();

        EXPECT_EQUAL(config.getAllBindAddresses().size(), expectedCount, "Wrong number of addresses");

        const std::vector<ServerBlock>& serverBlocks = config.getServerBlocks();
        const std::vector<const Ws::Sock::addr*>& sockAddr1 = serverBlocks[0].getListenAddresses();
        const std::vector<const Ws::Sock::addr*>& sockAddr2 = serverBlocks[1].getListenAddresses();

        EXPECT_EQUAL(sockAddr1.size(), expectedCount, "sockaddr was not correctly mapped to server 1");
        EXPECT_EQUAL(sockAddr2.size(), expectedCount, "sockaddr was not correctly mapped to server 2");

        EXPECT_EQUAL(sockAddr1[0], sockAddr2[0], "servers should share the same sockaddr for 0.0.0.0:80");
        EXPECT_EQUAL(sockAddr1[1], sockAddr2[1], "servers should share the same sockaddr for 123.123.123.123:80");

        char ip[INET_ADDRSTRLEN];
        Ws::Sock::addr_in* addr = (Ws::Sock::addr_in*)(sockAddr1[0]);
        ::inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN);

        EXPECT_EQUAL(::ntohs(addr->sin_port), 80, "Port mapping failed for first address");
        EXPECT_EQUAL(std::string(ip), std::string("0.0.0.0"), "IP mapping failed for first address");

        addr = (Ws::Sock::addr_in*)(sockAddr1[1]);
        ::inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN);

        EXPECT_EQUAL(::ntohs(addr->sin_port), 80, "Port mapping failed for second address");
        EXPECT_EQUAL(std::string(ip), std::string("123.123.123.123"), "IP mapping failed for second address");

        TEST_PASSED_MSG("two servers, two listeners, shared wildcard and concrete IPs");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

///////////////////////////////////////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);

        ServerConfig config("TwoServer_LocalHost.conf", defaultConfig);
        const size_t expectedCount = 1;

        config.parseConfigFile();

        EXPECT_EQUAL(config.getAllBindAddresses().size(), expectedCount, "Wrong number of addresses");

        const std::vector<ServerBlock>& serverBlocks = config.getServerBlocks();
        const std::vector<const Ws::Sock::addr*>& sockAddr1 = serverBlocks[0].getListenAddresses();
        const std::vector<const Ws::Sock::addr*>& sockAddr2 = serverBlocks[1].getListenAddresses();

        EXPECT_EQUAL(sockAddr1.size(), expectedCount, "sockaddr was not correctly mapped to server 1");
        EXPECT_EQUAL(sockAddr2.size(), expectedCount, "sockaddr was not correctly mapped to server 2");
        EXPECT_EQUAL(sockAddr1[0], sockAddr2[0], "both servers should point to the same localhost sockaddr");

        char ip[INET_ADDRSTRLEN];
        Ws::Sock::addr_in* addr = (Ws::Sock::addr_in*)(sockAddr1[0]);
        ::inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN);

        EXPECT_EQUAL(::ntohs(addr->sin_port), 80, "Port mapping failed");
        EXPECT_EQUAL(std::string(ip), std::string("127.0.0.1"), "IP mapping failed");

        TEST_PASSED_MSG("two servers, single listener, localhost");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

///////////////////////////////////////////////////////////////////////////////////////

    try
    {
        TEST_INTRO(testNumber++);

        ServerConfig config("OneServer_BadDNS.conf", defaultConfig);

                                            // setting pipes to read stderr
                                            int pipefd[2];
                                            int dupstrerr = dup(STDERR_FILENO);
                                            pipe(pipefd);
                                            dup2(pipefd[1], STDERR_FILENO);

        // test
        bool success = config.parseConfigFile();

                                            // reading from pipe
                                            char buffer[1024];
                                            int bytesRead = read(pipefd[0], buffer, 1024);
                                            buffer[bytesRead] = '\0';

                                            // restoring stderr
                                            dup2(dupstrerr, STDERR_FILENO);
                                            close(dupstrerr);
                                            close(pipefd[0]);
                                            close(pipefd[1]);

        std::string expected = "Error: DNS lookup failed for asfasfasfasfasfasfasfasfasf.com:80\n";

        EXPECT_EQUAL(success, false, "DNS lookup should fail");
        EXPECT_EQUAL(std::string(buffer), expected, "Error message is incorrect");

        TEST_PASSED_MSG("passing an IP that cannot be resolved");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

///////////////////////////////////////////////////////////////////////////////////////

    TEST_FOOTER;

    close(STDERR_FILENO);

    return (0);
}

