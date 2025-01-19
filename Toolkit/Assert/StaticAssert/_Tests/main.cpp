

#include "../StaticAssert.hpp"

int main()
{
    STATIC_ASSERT(sizeof(int) == 4, test1);     // pass
    //STATIC_ASSERT(sizeof(int) == 8, test2);   // fail, WON'T COMPILE
    return (0);
}