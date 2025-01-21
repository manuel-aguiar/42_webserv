


// C++ headers
# include <iostream>

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);

int main(void)
{
	int testNumber = 1;

	std::cout << "\n*************** EventManager tests ***************" << std::endl;

	testNumber = TestPart1(testNumber);
	testNumber = TestPart2(testNumber);

	std::cout << "**************************************************\n" << std::endl;

	return (0);
}

