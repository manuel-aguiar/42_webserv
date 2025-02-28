

#include "TestDependencies.hpp"
#include <arpa/inet.h>
#include <cstdlib>

#include "../../../../Toolkit/TestHelpers/TestHelpers.h"

// Helper function to create sockaddr
static struct sockaddr_in createSockAddr(const std::string& ip, const std::string& port) {

    struct sockaddr_in addr = (struct sockaddr_in){};

    addr.sin_family = AF_INET;

    // Convert port string to number
    addr.sin_port = htons(std::atoi(port.c_str()));

    // Handle IP
    if (ip.empty() || ip == "0.0.0.0") {
        addr.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0
    } else {
        inet_pton(AF_INET, ip.c_str(), &(addr.sin_addr));
    }

    return addr;
}

int main()
{

    TEST_HEADER("Server Block/BlockFinder Integration");
    int testNumber = 1;
////////////////////////////////////////////////////////////////////////////////////////////////////
    {
        try {
            TEST_INTRO(testNumber++);

            // Create a server block with specific configuration
            ServerBlock serverBlock;
            struct sockaddr_in addr = createSockAddr("127.0.0.1", "8080");
            serverBlock.addListenAddress((struct sockaddr*)&addr);
            serverBlock.addServerName("example.com");
            serverBlock.setRootPath("/var/www/html");
            serverBlock.setClientBodySize("10M");
            serverBlock.setClientHeaderSize("8K");
            serverBlock.addErrorPage("404:/errors/404.html");

            // Create BlockFinder and add the server block
            BlockFinder finder(2);
            finder.addServerBlock(serverBlock);

            // Test finding the block with exact match
            const ServerBlock* found = finder.findServerBlock(*(struct sockaddr*)&addr, "example.com");

            EXPECT_EQUAL(found != NULL, true, "Block not found with exact match");
            EXPECT_EQUAL(found->getRoot(), "/var/www/html", "Root path mismatch");
            EXPECT_EQUAL(found->getClientBodySize(), StringUtils::parse_size("10M"), "Client body size mismatch");

            TEST_PASSED_MSG("All configuration set");
        }
        catch (const std::exception& e) {
            TEST_FAILED_MSG(e.what());
        }
    }
////////////////////////////////////////////////////////////////////////////////////////////////////
    {
        try {

            TEST_INTRO(testNumber++);

            ServerBlock serverBlock;
            BlockFinder finder(2);

            // adding one more block with configuration set
            ServerBlock serverBlock2;
            struct sockaddr_in addr2 = createSockAddr("127.0.0.2", "1000");
            serverBlock2.addListenAddress((struct sockaddr*)&addr2);
            serverBlock2.addServerName("example-domain.com");
            serverBlock2.setRootPath("/var/www/html");
            serverBlock2.setClientBodySize("10M");
            serverBlock2.setClientHeaderSize("8K");
            serverBlock2.addErrorPage("404:/errors/404.html");
            finder.addServerBlock(serverBlock2);

            // with no configuration set, it should return the first block
            finder.addServerBlock(serverBlock);

            // now searching for the block with configuration set
            const ServerBlock* found = finder.findServerBlock(*(struct sockaddr*)&addr2, "example-domain.com");

            EXPECT_EQUAL(found == &serverBlock2, true, "Block should be found");

            TEST_PASSED_MSG("Searching for block with no configuration set");
        }
        catch (const std::exception& e) {
            TEST_FAILED_MSG(e.what());
        }
    }
////////////////////////////////////////////////////////////////////////////////////////////////////
    {
        try {
            TEST_INTRO(testNumber++);
            BlockFinder finder(4);

            // First block
            ServerBlock block1;
            struct sockaddr_in addr1 = createSockAddr("127.0.0.1", "8080");
            block1.addListenAddress((struct sockaddr*)&addr1);
            block1.addServerName("example.com");
            block1.setRootPath("/var/www/html1");
            finder.addServerBlock(block1);

            // Second block with same server name but different port
            ServerBlock block2;
            struct sockaddr_in addr2 = createSockAddr("127.0.0.1", "8081");
            block2.addListenAddress((struct sockaddr*)&addr2);
            block2.addServerName("example.com");
            block2.setRootPath("/var/www/html2");
            finder.addServerBlock(block2);

            // Test finding each block
            const ServerBlock* found1 = finder.findServerBlock(*(struct sockaddr*)&addr1, "example.com");
            const ServerBlock* found2 = finder.findServerBlock(*(struct sockaddr*)&addr2, "example.com");

            EXPECT_EQUAL(found1 != NULL, true, "First block not found");
            EXPECT_EQUAL(found1->getRoot(), "/var/www/html1", "First block found but, incorrect match");

            EXPECT_EQUAL(found2 != NULL, true, "Second block not found");
            EXPECT_EQUAL(found2->getRoot(), "/var/www/html2", "Second block found but, incorrect match");

            TEST_PASSED_MSG("Multiple blocks with same server name but different ports");
        }
        catch (const std::exception& e) {
            TEST_FAILED_MSG(e.what());
        }
    }
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////

    TEST_FOOTER;

    return 0;
}
