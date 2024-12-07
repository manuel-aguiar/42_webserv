#include "BlockFinder.hpp"
#include <iomanip>

void	printBlock(const ServerBlocks *block, t_ip_str ip, t_port_str port, t_server_name server_name)
{
    std::cout << "┌──────────── Block Found ────────────┐" << std::endl;
    std::cout << "│ ID:          " << std::left << std::setw(23) << block->id() << "│" << std::endl;
    std::cout << "│ IP:          " << std::left << std::setw(23) << ip << "│" << std::endl;
    std::cout << "│ Port:        " << std::left << std::setw(23) << port << "│" << std::endl;
    std::cout << "│ Server Name: " << std::left << std::setw(23) << server_name << "│" << std::endl;
    std::cout << "└─────────────────────────────────────┘\n\n" << std::endl;
}

void    checkIfFound(const ServerBlocks* block_found, const t_ip_str ip, const t_port_str port, const t_server_name server_name)
{
    if (block_found)
        printBlock(block_found, ip, port, server_name);
    else
        std::cout << "Block not found" << std::endl;
}

void    testPrecedence(BlockFinder& bfinder) {
    ServerBlocks block1("block1");
    ServerBlocks block2("block2");

    // Add blocks with different combinations of IP, port, and server name
    bfinder.addServerBlock(block1, "127.0.0.1", "80", "example.com");   // IP: 127.0.0.1, Port: 80, Server Name: example.com
    bfinder.addServerBlock(block2, "0.0.0.0", "80", "example.com");    // Wildcard IP, Port: 80, Server Name: example.com
    bfinder.addServerBlock(block2, "127.0.0.1", "80", "test.com");     // IP: 127.0.0.1, Port: 80, Server Name: test.com

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
    ServerBlocks	block("newTestBlock");

    bfinder.addServerBlock(block, "0.0.0.0", "443", "somedomain.com");
    checkIfFound(bfinder.findServerBlock("127.0.0.2", "443", "somedomain.com"), "0.0.0.0", "443", "somedomain.com");
}

int	main(int argc, char **argv)
{
    ServerConfig	config;
    ServerBlocks	block("block");

    BlockFinder	bfinder(config);

    testSingle(bfinder);

    // this is good data
    bfinder.addServerBlock(block, "127.0.0.2", "443", "somedomain.com");
    // same server config block but on wildcard ip
    bfinder.addServerBlock(block, "0.0.0.0", "443", "somedomain.com");

    std::cout << "Searching a block with exact ip, port and server_name" << std::endl;
    checkIfFound(bfinder.findServerBlock("127.0.0.2", "443", "somedomain.com"), "127.0.0.2", "443", "somedomain.com");

    // try adding empty strings, they will be replaced with the wildcard value
    std::cout << "Adding an empty ip" << std::endl;
    // this will be replaced with the wildcard value
    ServerBlocks	block_empty_ip("empty_ip");
    bfinder.addServerBlock(block_empty_ip, "", "80", "localhost");

    // lets check the ip wildcard block exists
    std::cout << "Searching a block with empty ip, which should have been replaced with the wildcard value" << std::endl;
    checkIfFound(bfinder.findServerBlock("", "80", "localhost"), "0.0.0.0", "80", "localhost");

    // try to add a block with the same ip, port and server_name
    ServerBlocks	block_duplicate("duplicate");
    bfinder.addServerBlock(block_duplicate, "", "80", "localhost"); // this will be discarded

    std::cout << "Searching a block with duplicate ip, port and server_name" << std::endl;
    checkIfFound(bfinder.findServerBlock("", "80", "localhost"), "0.0.0.0", "80", "localhost");

    std::cout << "Adding an empty server_name" << std::endl;
    // this will be replaced with the wildcard value
    bfinder.addServerBlock(block, "127.0.0.1", "80", "");

    std::cout << "Searching a block with empty server_name" << std::endl;
    checkIfFound(bfinder.findServerBlock("127.0.0.1", "80", ""), "127.0.0.1", "80", "");

    testPrecedence(bfinder);

    // can remove a block
    bfinder.removeServerBlock("127.0.0.1", "80", "");
    checkIfFound(bfinder.findServerBlock("127.0.0.1", "80", ""), "127.0.0.1", "80", "");

    // trigger manually
    if (argc > 1 && std::string(argv[1]) == "-wp") {
        // finally adding a wildcard port, which will throw an assertion failure
        bfinder.addServerBlock(block, "127.0.0.1", "*", "localhost");
    }

    return (0);
}
