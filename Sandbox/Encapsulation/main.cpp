


#include <iostream>


static void fillHexHeader(char* hexHeader, int hexheaderSize, int chunkSize)
{
    const char* hex = "0123456789abcdef";

    int i = hexheaderSize - 1;
    hexHeader[i--] = '\n';
    hexHeader[i--] = '\r';
    
    //

    while (i >= 0)
    {
        std::cout << i << std::endl;
        if (chunkSize)
        {
            hexHeader[i--] = hex[chunkSize % 16];
            chunkSize /= 16;
        }
        else
            hexHeader[i--] = '0';
    }
    std::cout << i << std::endl;
}


int main(void)
{
    char hexHeader[11] = {0};

    fillHexHeader(hexHeader, 10, 1024);

    std::cout << hexHeader << std::endl;
}