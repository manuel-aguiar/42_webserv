#include "BlockFinder.hpp"
#include <iomanip>

void	printBlock(const ServerBlock *block, t_ip_str ip, t_port_str port, t_server_name server_name)
{
    std::cout << "┌──────────── Block Found ────────────┐" << std::endl;
    std::cout << "│ ID:          " << std::left << std::setw(23) << block->id() << "│" << std::endl;
    std::cout << "│ IP:          " << std::left << std::setw(23) << ip << "│" << std::endl;
    std::cout << "│ Port:        " << std::left << std::setw(23) << port << "│" << std::endl;
    std::cout << "│ Server Name: " << std::left << std::setw(23) << server_name << "│" << std::endl;
    std::cout << "└─────────────────────────────────────┘\n\n" << std::endl;
}

void    checkIfFound(const ServerBlock* block_found, const t_ip_str ip, const t_port_str port, const t_server_name server_name)
{
    if (block_found)
        printBlock(block_found, ip, port, server_name);
    else
        std::cout << "Block not found" << std::endl;
}

void    testPrecedence(BlockFinder& bfinder) {
    ServerBlock block1("block1");
    ServerBlock block2("block2");

    // Add blocks with different combinations of IP, port, and server name
    block1.addListener("127.0.0.1", "80");
    block1.addServerName("example.com");
    bfinder.addServerBlock(block1);   // IP: 127.0.0.1, Port: 80, Server Name: example.com

    block2.addListener("0.0.0.0", "80");
    block2.addServerName("example.com");
    bfinder.addServerBlock(block2);    // Wildcard IP, Port: 80, Server Name: example.com

    block2.addListener("127.0.0.1", "80");
    block2.addServerName("test.com");
    bfinder.addServerBlock(block2);     // IP: 127.0.0.1, Port: 80, Server Name: test.com

    // Exact match on IP, port, and server name (should match block1)
    std::cout << "Searching for block with IP: 127.0.0.1, Port: 80, Server Name: example.com" << std::endl;
    checkIfFound(bfinder.findServerBlock("127.0.0.1", "80", "example.com"), "127.0.0.1", "80", "example.com");

    // Match by IP and Port, wildcard server name (should match block1)
    std::cout << "Searching for block with IP: 127.0.0.1, Port: 80, Server Name: *" << std::endl;
    checkIfFound(bfinder.findServerBlock("127.0.0.1", "80", "*"), "127.0.0.1", "80", "*");

    // Match by IP and Port, different server name (should match block2 with "test.com")
    std::cout << "Searching for block with IP: 127.0.0.1, Port: 80, Server Name: test.com" << std::endl;
    checkIfFound(bfinder.findServerBlock("127.0.0.1", "80", "test.com"), "127.0.0.1", "80", "test.com");

    // Match by wildcard IP and exact port and server name (should match block1)
    std::cout << "Searching for block with IP: 0.0.0.0, Port: 80, Server Name: example.com" << std::endl;
    checkIfFound(bfinder.findServerBlock("0.0.0.0", "80", "example.com"), "0.0.0.0", "80", "example.com");

    // Search for block with wildcard IP and port, wildcard server name (should match block1)
    std::cout << "Searching for block with IP: 0.0.0.0, Port: 80, Server Name: *" << std::endl;
    checkIfFound(bfinder.findServerBlock("0.0.0.0", "80", "*"), "0.0.0.0", "80", "*");
}

void    testSingle(BlockFinder& bfinder) {
    std::cout << "Testing adding a block with ip: 0.0.0.0, port: 443, server_name: somedomain.com" << std::endl;
    ServerBlock	block("newTestBlock");

    block.addListener("0.0.0.0", "443");
    block.addServerName("somedomain.com");
    bfinder.addServerBlock(block);
    checkIfFound(bfinder.findServerBlock("127.0.0.2", "443", "somedomain.com"), "0.0.0.0", "443", "somedomain.com");
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

            block1.addListener("0.0.0.0", "80");
            block1.addServerName("example.com");
            finder.addServerBlock(block1);

            block2.addListener("127.0.0.1", "80");
            block2.addServerName("example.com");
            finder.addServerBlock(block2);

            //test
            const ServerBlock* result = finder.findServerBlock("127.0.0.2", "80", "example.com");
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

        block2.addListener("127.0.0.1", "80");
        block2.addServerName("example.com");
        finder.addServerBlock(block2);
        std::cout << "adding next block" << std::endl;

        block1.addListener("0.0.0.0", "80");
        block1.addServerName("example.com");
        finder.addServerBlock(block1);

        // test
        const ServerBlock* result = finder.findServerBlock("127.0.0.1", "80", "example.com");
        const ServerBlock* expected = &block2;

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

        block1.addListener("127.0.0.1", "80");
        block1.addServerName("example.com");
        finder.addServerBlock(block1);

        //test
        const ServerBlock* result = finder.findServerBlock("127.0.0.2", "80", "example.com");
        const ServerBlock* expected = NULL;

        checkIfFound(result, "127.0.0.2", "80", "example.com");
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

        block1.addListener("0.0.0.0", "80");
        block1.addServerName("example.com");
        finder.addServerBlock(block1);

        block2.addListener("127.0.0.1", "80");
        block2.addServerName("example.com");
        finder.addServerBlock(block2);

        block3.addListener("127.0.0.2", "443");
        block3.addServerName("somedomain.com");
        finder.addServerBlock(block3);

        //test
        const ServerBlock* result = finder.findServerBlock("127.0.0.2", "443", "somedomain.com");
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

        block1.addListener("0.0.0.0", "443");
        block1.addServerName("somedomain.com");
        bfinder.addServerBlock(block1);

        //test
        const ServerBlock* result = bfinder.findServerBlock("127.0.0.2", "443", "somedomain.com");
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

        block1.addListener("0.0.0.0", "443");
        block1.addServerName("somedomain.com");
        bfinder.addServerBlock(block1);

        const ServerBlock* result = bfinder.findServerBlock("127.0.0.2", "444", "somedomain.com");
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
    block.addListener("127.0.0.2", "443");
    block.addServerName("somedomain.com");
    bfinder.addServerBlock(block);

    // same server config block but on wildcard ip
    block.addListener("0.0.0.0", "443");
    bfinder.addServerBlock(block);

    std::cout << "Searching a block with exact ip, port and server_name" << std::endl;
    checkIfFound(bfinder.findServerBlock("127.0.0.2", "443", "somedomain.com"), "127.0.0.2", "443", "somedomain.com");

    // try adding empty strings, they will be replaced with the wildcard value
    std::cout << "Adding an empty ip" << std::endl;
    // this will be replaced with the wildcard value
    ServerBlock	block_empty_ip("empty_ip");
    block_empty_ip.addListener("", "80");
    block_empty_ip.addServerName("localhost");
    bfinder.addServerBlock(block_empty_ip);

    // lets check the ip wildcard block exists
    std::cout << "Searching a block with empty ip, which should have been replaced with the wildcard value" << std::endl;
    checkIfFound(bfinder.findServerBlock("", "80", "localhost"), "0.0.0.0", "80", "localhost");

    // try to add a block with the same ip, port and server_name
    ServerBlock	block_duplicate("duplicate");
    block_duplicate.addListener("", "80");
    block_duplicate.addServerName("localhost");
    bfinder.addServerBlock(block_duplicate); // this will be discarded

    std::cout << "Searching a block with duplicate ip, port and server_name" << std::endl;
    checkIfFound(bfinder.findServerBlock("", "80", "localhost"), "0.0.0.0", "80", "localhost");

    std::cout << "Adding an empty server_name" << std::endl;
    // this will be replaced with the wildcard value
    block.addListener("127.0.0.1", "80");
    block.addServerName("");
    bfinder.addServerBlock(block);

    std::cout << "Searching a block with empty server_name" << std::endl;
    checkIfFound(bfinder.findServerBlock("127.0.0.1", "80", ""), "127.0.0.1", "80", "");

    testPrecedence(bfinder);

    // can remove a block
    bfinder.removeServerBlock("127.0.0.1", "80", "");
    checkIfFound(bfinder.findServerBlock("127.0.0.1", "80", ""), "127.0.0.1", "80", "");

    // trigger manually
    if (argc > 1 && std::string(argv[1]) == "-wp") {
        // finally adding a wildcard port, which will throw an assertion failure
        block.addListener("127.0.0.1", "*");
        block.addServerName("localhost");
        bfinder.addServerBlock(block);
    }

    return (0);
}