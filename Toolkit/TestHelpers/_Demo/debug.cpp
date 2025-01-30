
#include <iostream>

extern size_t mystrlen(const char* str);

int main(void)
{
	std::cout << "this is a freeform test, debug prints, here using strlen: " << mystrlen("Hello World!") << std::endl;

	return (0);
}