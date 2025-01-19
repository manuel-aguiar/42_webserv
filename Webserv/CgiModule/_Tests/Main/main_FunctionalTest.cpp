

# include <iostream>
# include <unistd.h>

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);
//extern int SpecificTest(int testNumber);

int main(void)
{
	int testNumber = 1;
	std::cout << "\n*************** CgiModule Functional Tests ***************" << std::endl;

	testNumber = TestPart1(testNumber);
	//testNumber = TestPart2(testNumber);
	//testNumber = SpecificTest(testNumber);

	close(STDERR_FILENO);



	std::cout << "***********************************************\n" << std::endl;
}
