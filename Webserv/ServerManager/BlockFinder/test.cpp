#include "BlockFinder.hpp"

int	main()
{
    ServerConfig	config;
    ServerBlocks	block;

    BlockFinder	bfinder(config);
    bfinder.addServerBlock(block, "127.0.0.1", "80", "localhost");

    {

        const ServerBlocks*	block_found = bfinder.findServerBlock("127.0.0.2", "80", "localhost");
        if (block_found)
            std::cout << "Block found" << std::endl;
        else
            std::cout << "Block not found" << std::endl;
    }

    {
        const ServerBlocks*	block_found = bfinder.findServerBlock("127.0.0.1", "80", "localhost");
        if (block_found)
            std::cout << "Block found" << std::endl;
        else
            std::cout << "Block not found" << std::endl;
    }

    return (0);
}
