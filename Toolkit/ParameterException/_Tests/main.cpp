

//Project headers
#include "../ParameterException.hpp"

//C++ headers
#include <iostream>

int main()
{
    try
    {
        throw ParameterException("main", "socket", "Invalid parameter");
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}
