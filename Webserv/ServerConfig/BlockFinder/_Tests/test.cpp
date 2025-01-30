#include "BlockFinder.hpp"
#include <iomanip>
#include <arpa/inet.h>
#include <cstdlib>

// Helper functions for testing
static struct sockaddr_in* createSockAddr(const std::string& ip, const std::string& port) {
    struct sockaddr_in* addr = new struct sockaddr_in();
    addr->sin_family = AF_INET;

    // Convert port string to number (simple atoi for testing)
    addr->sin_port = htons(std::atoi(port.c_str()));

    // Handle IP
    if (ip.empty() || ip == "0.0.0.0") {
        addr->sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0
    } else {
        inet_pton(AF_INET, ip.c_str(), &(addr->sin_addr));
    }

    return addr;
}

// Helper to convert sockaddr to string for display
static std::string getIpString(const struct sockaddr* addr) {
    char ipStr[INET_ADDRSTRLEN];
    struct sockaddr_in* ipv4 = (struct sockaddr_in*)addr;
    inet_ntop(AF_INET, &(ipv4->sin_addr), ipStr, INET_ADDRSTRLEN);
    return std::string(ipStr);
}

static std::string getPortString(const struct sockaddr* addr) {
    struct sockaddr_in* ipv4 = (struct sockaddr_in*)addr;
    return StringUtils::to_string(ntohs(ipv4->sin_port));
}

void	printBlock(const ServerBlock *block, const struct sockaddr* addr, const std::string& server_name)
{
    std::cout << "┌──────────── Block Found ────────────┐" << std::endl;
    std::cout << "│ ID:          " << std::left << std::setw(23) << block->id() << "│" << std::endl;
    std::cout << "│ IP:          " << std::left << std::setw(23) << getIpString(addr) << "│" << std::endl;
    std::cout << "│ Port:        " << std::left << std::setw(23) << getPortString(addr) << "│" << std::endl;
    std::cout << "│ Server Name: " << std::left << std::setw(23) << server_name << "│" << std::endl;
    std::cout << "└─────────────────────────────────────┘\n\n" << std::endl;
}

void    checkIfFound(const ServerBlock* block_found, const struct sockaddr* addr, const std::string& server_name)
{
    if (block_found)
        printBlock(block_found, addr, server_name);
    else
        std::cout << "Block not found" << std::endl;
}

void    testPrecedence(BlockFinder& bfinder) {
    ServerBlock block1("block1");
    ServerBlock block2("block2");

    // Create sockaddr structures
    struct sockaddr_in* addr1 = createSockAddr("127.0.0.1", "80");
    struct sockaddr_in* addr2 = createSockAddr("0.0.0.0", "80");

    // Add blocks with different combinations
    block1.addListenAddress((struct sockaddr*)addr1);
    block1.addServerName("example.com");
    bfinder.addServerBlock(block1);

    block2.addListenAddress((struct sockaddr*)addr2);
    block2.addServerName("example.com");
    bfinder.addServerBlock(block2);

    // Test cases remain the same, just update the function calls
    std::cout << "Searching for block with IP: 127.0.0.1, Port: 80, Server Name: example.com" << std::endl;
    checkIfFound(bfinder.findServerBlock((struct sockaddr*)addr1, "example.com"),
                (struct sockaddr*)addr1, "example.com");

    // Match by IP and Port, wildcard server name (should match block1)
    std::cout << "Searching for block with IP: 127.0.0.1, Port: 80, Server Name: *" << std::endl;
    checkIfFound(bfinder.findServerBlock((struct sockaddr*)addr1, "*"),
                (struct sockaddr*)addr1, "*");

    // Match by IP and Port, different server name (should match block2 with "test.com")
    std::cout << "Searching for block with IP: 127.0.0.1, Port: 80, Server Name: test.com" << std::endl;
    checkIfFound(bfinder.findServerBlock((struct sockaddr*)addr1, "test.com"),
                (struct sockaddr*)addr1, "test.com");

    // Match by wildcard IP and exact port and server name (should match block1)
    std::cout << "Searching for block with IP: 0.0.0.0, Port: 80, Server Name: example.com" << std::endl;
    checkIfFound(bfinder.findServerBlock((struct sockaddr*)addr2, "example.com"),
                (struct sockaddr*)addr2, "example.com");

    // Search for block with wildcard IP and port, wildcard server name (should match block1)
    std::cout << "Searching for block with IP: 0.0.0.0, Port: 80, Server Name: *" << std::endl;
    checkIfFound(bfinder.findServerBlock((struct sockaddr*)addr2, "*"),
                (struct sockaddr*)addr2, "*");

    // Cleanup
    delete addr1;
    delete addr2;
}

void    testSingle(BlockFinder& bfinder) {
    std::cout << "Testing adding a block with ip: 0.0.0.0, port: 443, server_name: somedomain.com" << std::endl;
    ServerBlock	block("newTestBlock");

    struct sockaddr_in* testAddr = createSockAddr("0.0.0.0", "443");
    block.addListenAddress((struct sockaddr*)testAddr);
    block.addServerName("somedomain.com");
    bfinder.addServerBlock(block);
    checkIfFound(bfinder.findServerBlock((struct sockaddr*)testAddr, "somedomain.com"),
                (struct sockaddr*)testAddr, "somedomain.com");
}

void reviewTests()
{
    {
        std::cout << "  Test1: no Specific match available (expected result: deliever wildcard): \n";
        try
        {
            //setup
            ServerBlock block1("block1");
            ServerBlock block2("block2");
            BlockFinder finder;

            struct sockaddr_in* addr1 = createSockAddr("0.0.0.0", "80");
            struct sockaddr_in* addr2 = createSockAddr("127.0.0.1", "80");

            block1.addListenAddress((struct sockaddr*)addr1);
            block1.addServerName("example.com");
            finder.addServerBlock(block1);

            block2.addListenAddress((struct sockaddr*)addr2);
            block2.addServerName("example.com");
            finder.addServerBlock(block2);

            //test
            const ServerBlock* result = finder.findServerBlock((struct sockaddr*)addr1, "example.com");
            const ServerBlock* expected = &block1;

            if (result != expected)
                throw std::runtime_error("Expected block1 to be found, no specific match available");
            std::cout << "      PASSED\n";
        }
        catch(const std::exception& e)
        {
            std::cerr << "      FAILED :" << e.what() << '\n';
        }
    }

    std::cout << "  Test2: Specific match available (expected result: deliever specific): \n";
    try
    {
        //setup
        ServerBlock block1("block1");
        ServerBlock block2("block2");
        BlockFinder finder;

        struct sockaddr_in* addr2 = createSockAddr("127.0.0.1", "80");
        block2.addListenAddress((struct sockaddr*)addr2);
        block2.addServerName("example.com");
        finder.addServerBlock(block2);
        std::cout << "adding next block" << std::endl;

        struct sockaddr_in* addr1 = createSockAddr("0.0.0.0", "80");
        block1.addListenAddress((struct sockaddr*)addr1);
        block1.addServerName("example.com");
        finder.addServerBlock(block1);

        // test
        const ServerBlock* result = finder.findServerBlock((struct sockaddr*)addr1, "example.com");
        const ServerBlock* expected = &block1;

        if (result != expected)
            throw std::runtime_error("Expected block2 to be found, most specific is available");
        std::cout << "      PASSED\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << "      FAILED: " << e.what() << '\n';
    }

    std::cout << "  Test3: no match available (expected result: NULL): \n";
    try
    {
        //setup
        ServerBlock block1("block1");
        BlockFinder finder;

        struct sockaddr_in* addr1 = createSockAddr("127.0.0.1", "80");
        block1.addListenAddress((struct sockaddr*)addr1);
        block1.addServerName("example.com");
        finder.addServerBlock(block1);

        //test
        const ServerBlock* result = finder.findServerBlock((struct sockaddr*)addr1, "example.com");
        const ServerBlock* expected = &block1;

        checkIfFound(result, (struct sockaddr*)addr1, "example.com");
        if (result != expected)
            throw std::runtime_error("Received a block but No block matches the port:ip combo");
        std::cout << "      PASSED\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << "      FAILED: " << e.what() << '\n';
    }

    std::cout << "  Test4: specific match test \n";
    try
    {
        //setup
        ServerBlock block1("block1");
        ServerBlock block2("block2");
        ServerBlock block3("block3");
        BlockFinder finder;

        struct sockaddr_in* addr1 = createSockAddr("0.0.0.0", "80");
        block1.addListenAddress((struct sockaddr*)addr1);
        block1.addServerName("example.com");
        finder.addServerBlock(block1);

        struct sockaddr_in* addr2 = createSockAddr("127.0.0.1", "80");
        block2.addListenAddress((struct sockaddr*)addr2);
        block2.addServerName("example.com");
        finder.addServerBlock(block2);

        struct sockaddr_in* addr3 = createSockAddr("127.0.0.2", "443");
        block3.addListenAddress((struct sockaddr*)addr3);
        block3.addServerName("somedomain.com");
        finder.addServerBlock(block3);

        //test
        const ServerBlock* result = finder.findServerBlock((struct sockaddr*)addr3, "somedomain.com");
        const ServerBlock* expected = &block3;

        if (result != expected)
            throw std::runtime_error("There is a specific match available");
        std::cout << "      PASSED\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << "      FAILED: " << e.what() << '\n';
    }

    std::cout << "  Test5: quick wildcard test\n";
    try
    {
        //setup
        ServerBlock block1("block1");
        BlockFinder bfinder;

        struct sockaddr_in* addr1 = createSockAddr("0.0.0.0", "443");
        block1.addListenAddress((struct sockaddr*)addr1);
        block1.addServerName("somedomain.com");
        bfinder.addServerBlock(block1);

        //test
        const ServerBlock* result = bfinder.findServerBlock((struct sockaddr*)addr1, "somedomain.com");
        const ServerBlock* expected = &block1;

        if (result != expected)
            throw std::runtime_error("Expected block1 to be found, no specific match available");
        std::cout << "      PASSED\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << "      FAILED: " << e.what() << '\n';
    }

    std::cout << "  Test6: quick wildcard test, different port\n";
    try
    {
        // setup
        ServerBlock block1("block1");
        BlockFinder bfinder;

        struct sockaddr_in* addr1 = createSockAddr("0.0.0.0", "443");
        block1.addListenAddress((struct sockaddr*)addr1);
        block1.addServerName("somedomain.com");
        bfinder.addServerBlock(block1);

        struct sockaddr_in* addr2 = createSockAddr("127.0.0.2", "444");
        const ServerBlock* result = bfinder.findServerBlock((struct sockaddr*)addr2, "somedomain.com");
        const ServerBlock* expected = NULL;

        if (result != expected)
            throw std::runtime_error("Expected nothing to be found, ports don't match");
        std::cout << "      PASSED\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << "      FAILED: " << e.what() << '\n';
    }

    std::cerr << "Review tests completed\n";
}

int	main(int argc, char **argv)
{
    ServerBlock	block("block");
    BlockFinder	bfinder;

    reviewTests();
    testSingle(bfinder);

    // this is good data
    struct sockaddr_in* testAddr = createSockAddr("127.0.0.2", "443");
    block.addListenAddress((struct sockaddr*)testAddr);
    block.addServerName("somedomain.com");
    bfinder.addServerBlock(block);

    // same server config block but on wildcard ip
    struct sockaddr_in* addr = createSockAddr("0.0.0.0", "443");
    block.addListenAddress((struct sockaddr*)addr);
    bfinder.addServerBlock(block);

    std::cout << "Searching a block with exact ip, port and server_name" << std::endl;
    checkIfFound(bfinder.findServerBlock((struct sockaddr*)testAddr, "somedomain.com"),
                (struct sockaddr*)testAddr, "somedomain.com");

    // try adding empty strings, they will be replaced with the wildcard value
    std::cout << "Adding an empty ip" << std::endl;
    // this will be replaced with the wildcard value
    ServerBlock	block_empty_ip("empty_ip");
    struct sockaddr_in* emptyAddr = createSockAddr("", "80");
    block_empty_ip.addListenAddress((struct sockaddr*)emptyAddr);
    block_empty_ip.addServerName("localhost");
    bfinder.addServerBlock(block_empty_ip);

    // lets check the ip wildcard block exists
    std::cout << "Searching a block with empty ip, which should have been replaced with the wildcard value" << std::endl;
    checkIfFound(bfinder.findServerBlock((struct sockaddr*)emptyAddr, "localhost"),
                (struct sockaddr*)emptyAddr, "localhost");

    // try to add a block with the same ip, port and server_name
    ServerBlock	block_duplicate("duplicate");
    struct sockaddr_in* dupAddr = createSockAddr("", "80");
    block_duplicate.addListenAddress((struct sockaddr*)dupAddr);
    block_duplicate.addServerName("localhost");
    bfinder.addServerBlock(block_duplicate); // this will be discarded

    std::cout << "Searching a block with duplicate ip, port and server_name" << std::endl;
    checkIfFound(bfinder.findServerBlock((struct sockaddr*)dupAddr, "localhost"),
                (struct sockaddr*)dupAddr, "localhost");

    std::cout << "Adding an empty server_name" << std::endl;
    // this will be replaced with the wildcard value
    block.addListenAddress((struct sockaddr*)addr);
    block.addServerName("");
    bfinder.addServerBlock(block);

    std::cout << "Searching a block with empty server_name" << std::endl;
    checkIfFound(bfinder.findServerBlock((struct sockaddr*)addr, ""),
                (struct sockaddr*)addr, "");

    testPrecedence(bfinder);

    // can remove a block
    bfinder.removeServerBlock((struct sockaddr*)addr, "");
    checkIfFound(bfinder.findServerBlock((struct sockaddr*)addr, ""),
                (struct sockaddr*)addr, "");

    struct sockaddr_in* wildcardAddr = NULL;

    // trigger manually
    if (argc > 1 && std::string(argv[1]) == "-wp") {
        // finally adding a wildcard port, which will throw an assertion failure
        wildcardAddr = createSockAddr("127.0.0.1", "*");
        block.addListenAddress((struct sockaddr*)wildcardAddr);
        block.addServerName("localhost");
        bfinder.addServerBlock(block);
    }

    // Cleanup
    delete testAddr;
    delete addr;
    delete emptyAddr;
    delete dupAddr;
    if (wildcardAddr)
        delete wildcardAddr;

    return (0);
}