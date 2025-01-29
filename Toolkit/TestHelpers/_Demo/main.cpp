

#include "../TestHelpers.h"
#include <iostream>

int main(void)
{
    try
    {
        TestHelpers::assertEqual(2 + 2 , 2 + 3, "The result must be 5", __FILE__, __LINE__, __FUNCTION__);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    std::string errorMsg = TestHelpers::errorMsg(false, true, "The result must be true!", __FILE__, __LINE__, __FUNCTION__);

    std::cout << errorMsg << std::endl;

    return (0);
}