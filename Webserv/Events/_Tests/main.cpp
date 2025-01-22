


// C++ headers
# include <iostream>

//C headers
# include <unistd.h>

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);
extern int TestPart3(int testNumber);

int main(void)
{
	int testNumber = 1;

	std::cout << "\n*************** Manager tests ***************" << std::endl;

	testNumber = TestPart1(testNumber);
	testNumber = TestPart2(testNumber);
	testNumber = TestPart3(testNumber);

	::close(STDOUT_FILENO);

	std::cout << "**************************************************\n" << std::endl;

	return (0);
}

