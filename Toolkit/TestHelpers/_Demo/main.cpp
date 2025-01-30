

#include "../TestHelpers.h"
#include <iostream>

int main(void)
{
    try
    {
        EXPECT_EQUAL(2 + 2 , 2 + 3, "The result must be 5");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    std::string errorMsg = TEST_ERROR_MSG((int)4, 5, "The result must be true!");

    std::cout << errorMsg << std::endl;

    return (0);
}