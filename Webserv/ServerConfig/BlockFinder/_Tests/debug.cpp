
#include "../../ServerBlock/ServerBlock.hpp"
#include "../BlockFinder.hpp"
#include <iomanip>
#include <arpa/inet.h>
#include <cstdlib>

#include "../../../../Toolkit/TestHelpers/TestHelpers.h"

extern void	    printBlock(const ServerBlock *block, const Ws::Sock::addr* addr, const std::string& server_name);
extern void    checkIfFound(const ServerBlock* block_found, const Ws::Sock::addr* addr, const std::string& server_name);
Ws::Sock::addr_in createSockAddr(const std::string& ip, const std::string& port);
extern void reviewTests();



void    testPrecedence(BlockFinder& bfinder) {
    ServerBlock block1;
    ServerBlock block2;

    // Create sockaddr structures
    Ws::Sock::addr_in addr1 = createSockAddr("127.0.0.1", "80");
    Ws::Sock::addr_in addr2 = createSockAddr("0.0.0.0", "80");

    // Add blocks with different combinations
    block1.addListenAddress((Ws::Sock::addr*)&addr1);
    block1.addServerName("example.com");
    bfinder.addServerBlock(block1);

    block2.addListenAddress((Ws::Sock::addr*)&addr2);
    block2.addServerName("example.com");
    bfinder.addServerBlock(block2);

    // Test cases remain the same, just update the function calls
    std::cout << "Searching for block with IP: 127.0.0.1, Port: 80, Server Name: example.com" << std::endl;
    checkIfFound(bfinder.findServerBlock(*(Ws::Sock::addr*)&addr1, "example.com"),
                (Ws::Sock::addr*)&addr1, "example.com");

    // Match by IP and Port, wildcard server name (should match block1)
    std::cout << "Searching for block with IP: 127.0.0.1, Port: 80, Server Name: *" << std::endl;
    checkIfFound(bfinder.findServerBlock(*(Ws::Sock::addr*)&addr1, "*"),
                (Ws::Sock::addr*)&addr1, "*");

    // Match by IP and Port, different server name (should match block2 with "test.com")
    std::cout << "Searching for block with IP: 127.0.0.1, Port: 80, Server Name: test.com" << std::endl;
    checkIfFound(bfinder.findServerBlock(*(Ws::Sock::addr*)&addr1, "test.com"),
                (Ws::Sock::addr*)&addr1, "test.com");

    // Match by wildcard IP and exact port and server name (should match block1)
    std::cout << "Searching for block with IP: 0.0.0.0, Port: 80, Server Name: example.com" << std::endl;
    checkIfFound(bfinder.findServerBlock(*(Ws::Sock::addr*)&addr2, "example.com"),
                (Ws::Sock::addr*)&addr2, "example.com");

    // Search for block with wildcard IP and port, wildcard server name (should match block1)
    std::cout << "Searching for block with IP: 0.0.0.0, Port: 80, Server Name: *" << std::endl;
    checkIfFound(bfinder.findServerBlock(*(Ws::Sock::addr*)&addr2, "*"),
                (Ws::Sock::addr*)&addr2, "*");

}

void    testSingle(BlockFinder& bfinder) {
    std::cout << "Testing adding a block with ip: 0.0.0.0, port: 443, server_name: somedomain.com" << std::endl;
    ServerBlock	block;

    Ws::Sock::addr_in testAddr = createSockAddr("0.0.0.0", "443");
    block.addListenAddress((Ws::Sock::addr*)&testAddr);
    block.addServerName("somedomain.com");
    bfinder.addServerBlock(block);
    checkIfFound(bfinder.findServerBlock(*(Ws::Sock::addr*)&testAddr, "somedomain.com"),
                (Ws::Sock::addr*)&testAddr, "somedomain.com");
}



int	main(void)
{
    ServerBlock	block;

    BlockFinder	bfinder1(1000); // just a number to fit all
    testSingle(bfinder1);

    BlockFinder	bfinder2(1000); 
    testPrecedence(bfinder2);

    std::cerr << "Review tests started\n";
    reviewTests();
    std::cerr << "Review tests completed\n";

    

    BlockFinder	bfinder(1000);
    // this is good data
    Ws::Sock::addr_in testAddr = createSockAddr("127.0.0.2", "443");
    block.addListenAddress((Ws::Sock::addr*)&testAddr);
    block.addServerName("somedomain.com");
    bfinder.addServerBlock(block);

    // same server config block but on wildcard ip
    Ws::Sock::addr_in addr = createSockAddr("0.0.0.0", "443");
    block.addListenAddress((Ws::Sock::addr*)&addr);
    bfinder.addServerBlock(block);

    std::cout << "Searching a block with exact ip, port and server_name" << std::endl;
    checkIfFound(bfinder.findServerBlock(*(Ws::Sock::addr*)&testAddr, "somedomain.com"),
                (Ws::Sock::addr*)&testAddr, "somedomain.com");

    // try adding empty strings, they will be replaced with the wildcard value
    std::cout << "Adding an empty ip" << std::endl;
    // this will be replaced with the wildcard value
    ServerBlock	block_empty_ip;
    Ws::Sock::addr_in emptyAddr = createSockAddr("", "80");
    block_empty_ip.addListenAddress((Ws::Sock::addr*)&emptyAddr);
    block_empty_ip.addServerName("localhost");
    bfinder.addServerBlock(block_empty_ip);

    // lets check the ip wildcard block exists
    std::cout << "Searching a block with empty ip, which should have been replaced with the wildcard value" << std::endl;
    checkIfFound(bfinder.findServerBlock(*(Ws::Sock::addr*)&emptyAddr, "localhost"),
                (Ws::Sock::addr*)&emptyAddr, "localhost");

    // try to add a block with the same ip, port and server_name
    ServerBlock	block_duplicate;
    Ws::Sock::addr_in dupAddr = createSockAddr("", "80");
    block_duplicate.addListenAddress((Ws::Sock::addr*)&dupAddr);
    block_duplicate.addServerName("localhost");
    bfinder.addServerBlock(block_duplicate); // this will be discarded

    std::cout << "Searching a block with duplicate ip, port and server_name" << std::endl;
    checkIfFound(bfinder.findServerBlock(*(Ws::Sock::addr*)&dupAddr, "localhost"),
                (Ws::Sock::addr*)&dupAddr, "localhost");

    std::cout << "Adding an empty server_name" << std::endl;
    // this will be replaced with the wildcard value
    block.addListenAddress((Ws::Sock::addr*)&addr);
    block.addServerName("");
    bfinder.addServerBlock(block);

    std::cout << "Searching a block with empty server_name" << std::endl;
    checkIfFound(bfinder.findServerBlock(*(Ws::Sock::addr*)&addr, ""),
                (Ws::Sock::addr*)&addr, "");

    return (0);
}