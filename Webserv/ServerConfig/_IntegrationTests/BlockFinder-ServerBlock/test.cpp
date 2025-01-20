#include "TestDependencies.hpp"
#include <arpa/inet.h>

// Helper function to create sockaddr
static struct sockaddr_in* createSockAddr(const std::string& ip, const std::string& port) {
    struct sockaddr_in* addr = new struct sockaddr_in();
    addr->sin_family = AF_INET;

    // Convert port string to number
    addr->sin_port = htons(std::atoi(port.c_str()));

    // Handle IP
    if (ip.empty() || ip == "0.0.0.0") {
        addr->sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0
    } else {
        inet_pton(AF_INET, ip.c_str(), &(addr->sin_addr));
    }

    return addr;
}

int main() {
    std::cout << "\n─ Server Block/BlockFinder Integration Test ─\n\n";
    {
        try {
            std::cout << "─ Test 1: All configuration set ─\n";
            // Create a server block with specific configuration
            ServerBlock serverBlock;
            struct sockaddr_in* addr = createSockAddr("127.0.0.1", "8080");
            serverBlock.addListenAddress((struct sockaddr*)addr);
            serverBlock.addServerName("example.com");
            serverBlock.setRootPath("/var/www/html");
            serverBlock.setClientBodySize("10M");
            serverBlock.setClientHeaderSize("8K");
            serverBlock.addErrorPage("404:/errors/404.html");

            // Create BlockFinder and add the server block
            BlockFinder finder;
            finder.addServerBlock(serverBlock);

            // Test finding the block with exact match
            const ServerBlock* found = finder.findServerBlock((struct sockaddr*)addr, "example.com");
            if (!found) {
                delete addr;
                throw std::runtime_error("Block not found with exact match");
            }

            // Verify the found block has the correct configuration
            if (found->getRoot() != "/var/www/html") {
                delete addr;
                throw std::runtime_error("Root path mismatch");
            }

            if (found->getClientBodySize() != StringUtils::parse_size("10M")) {
                delete addr;
                throw std::runtime_error("Client body size mismatch");
            }

            // Test finding with non-existent configuration
            struct sockaddr_in* addr2 = createSockAddr("127.0.0.2", "8080");
            const ServerBlock* notFound = finder.findServerBlock((struct sockaddr*)addr2, "example.com");
            if (notFound) {
                delete addr;
                delete addr2;
                throw std::runtime_error("Found block that shouldn't exist");
            }
            delete addr2;

            std::cout << "PASSED\n";
            delete addr;
        }
        catch (const std::exception& e) {
            std::cout << "FAILED: Exception: " << e.what() << "\n";
        }
        std::cout << "───────────────────────────────────\n";
    }

    {
        try {
            std::cout << "─ Test 2: Searching for block with no configuration set ─\n";
            ServerBlock serverBlock;
            BlockFinder finder;

            // adding one more block with configuration set
            ServerBlock serverBlock2;
            struct sockaddr_in* addr2 = createSockAddr("127.0.0.2", "1000");
            serverBlock2.addListenAddress((struct sockaddr*)addr2);
            serverBlock2.addServerName("example-domain.com");
            serverBlock2.setRootPath("/var/www/html");
            serverBlock2.setClientBodySize("10M");
            serverBlock2.setClientHeaderSize("8K");
            serverBlock2.addErrorPage("404:/errors/404.html");
            finder.addServerBlock(serverBlock2);

            // with no configuration set, it should return the first block
            finder.addServerBlock(serverBlock);

            // searching for the block with no configuration set
            struct sockaddr_in* addr = createSockAddr("127.0.0.1", "8080");
            const ServerBlock* found = finder.findServerBlock((struct sockaddr*)addr, "example.com");
            if (found != NULL) {
                delete addr;
                throw std::runtime_error("Found block that shouldn't exist");
            }

            // now searching for the block with configuration set
            found = finder.findServerBlock((struct sockaddr*)addr2, "example-domain.com");
            if (found == NULL || found != &serverBlock2) {
                delete addr;
                delete addr2;
                throw std::runtime_error("Block should be found");
            }

            delete addr;
            delete addr2;
            std::cout << "PASSED\n";
        }
        catch (const std::exception& e) {
            std::cout << "FAILED: Exception: " << e.what() << "\n";
        }
        std::cout << "───────────────────────────────────\n";
    }

    {
        try {
            std::cout << "─ Test 3: Multiple blocks with same server name but different ports ─\n";
            BlockFinder finder;

            // First block
            ServerBlock block1;
            struct sockaddr_in* addr1 = createSockAddr("127.0.0.1", "8080");
            block1.addListenAddress((struct sockaddr*)addr1);
            block1.addServerName("example.com");
            block1.setRootPath("/var/www/html1");
            finder.addServerBlock(block1);

            // Second block with same server name but different port
            ServerBlock block2;
            struct sockaddr_in* addr2 = createSockAddr("127.0.0.1", "8081");
            block2.addListenAddress((struct sockaddr*)addr2);
            block2.addServerName("example.com");
            block2.setRootPath("/var/www/html2");
            finder.addServerBlock(block2);

            // Test finding each block
            const ServerBlock* found1 = finder.findServerBlock((struct sockaddr*)addr1, "example.com");
            const ServerBlock* found2 = finder.findServerBlock((struct sockaddr*)addr2, "example.com");

            if (!found1 || found1->getRoot() != "/var/www/html1") {
                delete addr1;
                delete addr2;
                throw std::runtime_error("First block not found or incorrect");
            }
            if (!found2 || found2->getRoot() != "/var/www/html2") {
                delete addr1;
                delete addr2;
                throw std::runtime_error("Second block not found or incorrect");
            }

            delete addr1;
            delete addr2;
            std::cout << "PASSED\n";
        }
        catch (const std::exception& e) {
            std::cout << "FAILED: Exception: " << e.what() << "\n";
        }
        std::cout << "───────────────────────────────────\n";
    }

    {
        try {
            std::cout << "─ Test 4: Wildcard IP with specific server name ─\n";
            BlockFinder finder;

            // Block with wildcard IP
            ServerBlock block;
            struct sockaddr_in* addr = createSockAddr("0.0.0.0", "8080");
            block.addListenAddress((struct sockaddr*)addr);
            block.addServerName("example.com");
            block.setRootPath("/var/www/html");
            finder.addServerBlock(block);

            // Test finding with different IPs
            struct sockaddr_in* addr1 = createSockAddr("127.0.0.1", "8080");
            const ServerBlock* found1 = finder.findServerBlock((struct sockaddr*)addr1, "example.com");
            struct sockaddr_in* addr2 = createSockAddr("192.168.1.1", "8080");
            const ServerBlock* found2 = finder.findServerBlock((struct sockaddr*)addr2, "example.com");

            if (!found1 || !found2) {
                delete addr;
                delete addr1;
                delete addr2;
                throw std::runtime_error("Block not found with wildcard IP");
            }
            if (found1 != found2) {
                delete addr;
                delete addr1;
                delete addr2;
                throw std::runtime_error("Different blocks returned for same wildcard");
            }

            delete addr;
            delete addr1;
            delete addr2;
            std::cout << "PASSED\n";
        }
        catch (const std::exception& e) {
            std::cout << "FAILED: Exception: " << e.what() << "\n";
        }
        std::cout << "───────────────────────────────────\n";
    }

    {
        try {
            std::cout << "─ Test 5: Multiple server names for same block ─\n";
            BlockFinder finder;

            // Block with multiple server names
            ServerBlock block;
            struct sockaddr_in* addr = createSockAddr("127.0.0.1", "8080");
            block.addListenAddress((struct sockaddr*)addr);
            block.addServerName("example.com");
            block.addServerName("www.example.com");
            block.addServerName("api.example.com");
            block.setRootPath("/var/www/html");
            finder.addServerBlock(block);

            // Test finding with different server names
            const ServerBlock* found1 = finder.findServerBlock((struct sockaddr*)addr, "example.com");
            const ServerBlock* found2 = finder.findServerBlock((struct sockaddr*)addr, "www.example.com");
            const ServerBlock* found3 = finder.findServerBlock((struct sockaddr*)addr, "api.example.com");

            if (!found1 || !found2 || !found3) {
                delete addr;
                throw std::runtime_error("Block not found for one of the server names");
            }
            if (found1 != found2 || found2 != found3) {
                delete addr;
                throw std::runtime_error("Different blocks returned for same configuration");
            }

            delete addr;
            std::cout << "PASSED\n";
        }
        catch (const std::exception& e) {
            std::cout << "FAILED: Exception: " << e.what() << "\n";
        }
        std::cout << "───────────────────────────────────\n";
    }

    return 0;
}
