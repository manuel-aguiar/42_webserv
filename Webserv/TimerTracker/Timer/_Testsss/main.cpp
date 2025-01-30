

#include "../Timer.hpp"
# include <iostream>

int main(void)
{
    Timer timer = Timer::now();

    std::cout << "Seconds: " << timer.getSeconds() << std::endl;

    timer += 5000;

    std::cout << "Seconds: " << timer.getSeconds() << std::endl;
    return (0);
}