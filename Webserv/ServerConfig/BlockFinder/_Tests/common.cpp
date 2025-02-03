

// target
#include "../BlockFinder.hpp"

// Project headers
#include "../../ServerBlock/ServerBlock.hpp"
#include "../../../GenericUtils/StringUtils/StringUtils.hpp"    
#include "../../../../Toolkit/TestHelpers/TestHelpers.h"

// C++ headers
#include <iomanip>
#include <arpa/inet.h>
#include <cstdlib>

// Helper to convert sockaddr to string for display
static std::string getIpString(const Ws::Sock::addr* addr) {
    char ipStr[INET_ADDRSTRLEN];
    Ws::Sock::addr_in* ipv4 = (Ws::Sock::addr_in*)addr;
    inet_ntop(AF_INET, &(ipv4->sin_addr), ipStr, INET_ADDRSTRLEN);
    return std::string(ipStr);
}

static std::string getPortString(const Ws::Sock::addr* addr) {
    Ws::Sock::addr_in* ipv4 = (Ws::Sock::addr_in*)addr;
    return StringUtils::to_string(ntohs(ipv4->sin_port));
}


void	printBlock(const ServerBlock *block, const Ws::Sock::addr* addr, const std::string& server_name)
{
    std::cout << "┌──────────── Block Found ────────────┐" << std::endl;
    std::cout << "│ ID:          " << std::left << std::setw(23) << block << "│" << std::endl;
    std::cout << "│ IP:          " << std::left << std::setw(23) << getIpString(addr) << "│" << std::endl;
    std::cout << "│ Port:        " << std::left << std::setw(23) << getPortString(addr) << "│" << std::endl;
    std::cout << "│ Server Name: " << std::left << std::setw(23) << server_name << "│" << std::endl;
    std::cout << "└─────────────────────────────────────┘\n\n" << std::endl;
}

void    checkIfFound(const ServerBlock* block_found, const Ws::Sock::addr* addr, const std::string& server_name)
{
    if (block_found)
        printBlock(block_found, addr, server_name);
    else
        std::cout << "Block not found" << std::endl;
}

// Helper functions for testing
Ws::Sock::addr_in createSockAddr(const std::string& ip, const std::string& port) {
    Ws::Sock::addr_in addr = (Ws::Sock::addr_in){};
    addr.sin_family = AF_INET;

    // Convert port string to number (simple atoi for testing)
    addr.sin_port = htons(std::atoi(port.c_str()));

    // Handle IP
    if (ip.empty() || ip == "0.0.0.0") {
        addr.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0
    } else {
        inet_pton(AF_INET, ip.c_str(), &(addr.sin_addr));
    }

    return addr;
}

void reviewTests()
{
    int testNumber = 1;

    {
        TEST_INTRO(testNumber++);
        try
        {
            //setup
            ServerBlock block1;
            ServerBlock block2;
            BlockFinder finder;

            Ws::Sock::addr_in addr1 = createSockAddr("0.0.0.0", "80");
            Ws::Sock::addr_in addr2 = createSockAddr("127.0.0.1", "80");

            block1.addListenAddress((Ws::Sock::addr*)&addr1);
            block1.addServerName("example.com");
            finder.addServerBlock(block1);

            block2.addListenAddress((Ws::Sock::addr*)&addr2);
            block2.addServerName("example.com");
            finder.addServerBlock(block2);

            EXPECT_EQUAL(finder.findServerBlock((Ws::Sock::addr*)&addr1, "example.com"), &block1, 
                        "Expected block1 to be found, no specific match available");

            TEST_PASSED_MSG("no Specific match available (expected result: deliever wildcard)");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }


    TEST_INTRO(testNumber++);
    try
    {
        //setup
        ServerBlock block1;
        ServerBlock block2;
        BlockFinder finder;

        Ws::Sock::addr_in addr2 = createSockAddr("127.0.0.1", "80");
        block2.addListenAddress((Ws::Sock::addr*)&addr2);
        block2.addServerName("example.com");
        finder.addServerBlock(block2);

        Ws::Sock::addr_in addr1 = createSockAddr("0.0.0.0", "80");
        block1.addListenAddress((Ws::Sock::addr*)&addr1);
        block1.addServerName("example.com");
        finder.addServerBlock(block1);

        // test
        EXPECT_EQUAL(finder.findServerBlock((Ws::Sock::addr*)&addr2, "example.com"), &block2, 
                        "Expected block2 to be found, most speciific match");

        TEST_PASSED_MSG("Specific match available (expected result: deliever specific)");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    TEST_INTRO(testNumber++);
    try
    {
        //setup
        ServerBlock block1;
        BlockFinder finder;

        Ws::Sock::addr_in addr1 = createSockAddr("127.0.0.1", "80");
        block1.addListenAddress((Ws::Sock::addr*)&addr1);
        block1.addServerName("example.com");
        finder.addServerBlock(block1);

        //test
        EXPECT_EQUAL(finder.findServerBlock((Ws::Sock::addr*)&addr1, "yoyoyo.com"), (ServerBlock*)NULL,
                    "Expected no block to be found, no match available");
        
        TEST_PASSED_MSG("no match available (expected result: NULL)");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    TEST_INTRO(testNumber++);
    try
    {
        //setup
        ServerBlock block1;
        ServerBlock block2;
        ServerBlock block3;
        BlockFinder finder;

        Ws::Sock::addr_in addr1 = createSockAddr("0.0.0.0", "80");
        block1.addListenAddress((Ws::Sock::addr*)&addr1);
        block1.addServerName("example.com");
        finder.addServerBlock(block1);

        Ws::Sock::addr_in addr2 = createSockAddr("127.0.0.1", "80");
        block2.addListenAddress((Ws::Sock::addr*)&addr2);
        block2.addServerName("example.com");
        finder.addServerBlock(block2);

        Ws::Sock::addr_in addr3 = createSockAddr("127.0.0.2", "443");
        block3.addListenAddress((Ws::Sock::addr*)&addr3);
        block3.addServerName("somedomain.com");
        finder.addServerBlock(block3);

        //test
        EXPECT_EQUAL(finder.findServerBlock((Ws::Sock::addr*)&addr3, "somedomain.com"), &block3, 
                        "Expected block3 to be found, most specific match");

        TEST_PASSED_MSG("specific match test");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    TEST_INTRO(testNumber++);
    try
    {
        //setup
        ServerBlock block1;
        BlockFinder bfinder;

        Ws::Sock::addr_in addr1 = createSockAddr("0.0.0.0", "443");
        block1.addListenAddress((Ws::Sock::addr*)&addr1);
        block1.addServerName("somedomain.com");
        bfinder.addServerBlock(block1);

        //test

        EXPECT_EQUAL(bfinder.findServerBlock((Ws::Sock::addr*)&addr1, "somedomain.com"), &block1, 
                        "Expected block1 to be found, no specific match available");

        TEST_PASSED_MSG("quick wildcard test");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    TEST_INTRO(testNumber++);
    try
    {
        // setup
        ServerBlock block1;
        BlockFinder bfinder;

        Ws::Sock::addr_in addr1 = createSockAddr("0.0.0.0", "443");
        block1.addListenAddress((Ws::Sock::addr*)&addr1);
        block1.addServerName("somedomain.com");
        bfinder.addServerBlock(block1);

        Ws::Sock::addr_in addr2 = createSockAddr("127.0.0.2", "444");

        EXPECT_EQUAL(bfinder.findServerBlock((Ws::Sock::addr*)&addr2, "somedomain.com"), (ServerBlock*)NULL, 
                        "Expected no block to be found, ports don't match");
        
        TEST_PASSED_MSG("quick wildcard test, different port");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

}