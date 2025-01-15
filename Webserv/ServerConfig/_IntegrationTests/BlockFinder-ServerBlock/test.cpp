#include "../../ServerBlock/ServerBlock.hpp"
#include "../../BlockFinder/BlockFinder.hpp"
#include <iostream>
#include <iomanip>

void printTestHeader(const std::string& testName) {
    std::cout << "\n┌──────────── " << testName << " ────────────┐\n";
}

void printTestResult(bool passed) {
    if (passed)
        std::cout << "│ Result: PASSED" << std::setw(31) << "│\n";
    else
        std::cout << "│ Result: FAILED" << std::setw(31) << "│\n";
    std::cout << "└─────────────────────────────────────┘\n";
}

bool testServerBlockIntegration() {
    printTestHeader("Server Block Integration Test");
    try {
        // Create a server block with specific configuration
        ServerBlock serverBlock;
        serverBlock.addListener("127.0.0.1:8080");
        serverBlock.addServerName("example.com");
        serverBlock.setRootPath("/var/www/html");
        serverBlock.setClientBodySize("10M");
        serverBlock.setClientHeaderSize("8K");
        serverBlock.addErrorPage("404:/errors/404.html");

        // Create BlockFinder and add the server block
        BlockFinder finder;
        finder.addServerBlock(serverBlock);

        // Test finding the block with exact match
        const ServerBlock* found = finder.findServerBlock("127.0.0.1", "8080", "example.com");
        if (!found) {
            std::cout << "│ Error: Block not found with exact match" << std::setw(8) << "│\n";
            return false;
        }

        // Verify the found block has the correct configuration
        if (found->getRoot() != "/var/www/html") {
            std::cout << "│ Error: Root path mismatch" << std::setw(20) << "│\n";
            return false;
        }

        if (found->getClientBodySize() != StringUtils::parse_size("10M")) {
            std::cout << "│ Error: Client body size mismatch" << std::setw(14) << "│\n";
            return false;
        }

        // Test finding with non-existent configuration
        const ServerBlock* notFound = finder.findServerBlock("127.0.0.2", "8080", "example.com");
        if (notFound) {
            std::cout << "│ Error: Found block that shouldn't exist" << std::setw(9) << "│\n";
            return false;
        }

        return true;
    }
    catch (const std::exception& e) {
        std::cout << "│ Exception: " << e.what() << std::setw(40 - strlen(e.what())) << "│\n";
        return false;
    }
}

int main() {
    bool testPassed = testServerBlockIntegration();
    printTestResult(testPassed);
    return testPassed ? 0 : 1;
}