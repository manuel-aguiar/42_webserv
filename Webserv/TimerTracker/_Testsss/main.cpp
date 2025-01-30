

# include <iostream>

extern int TestPart1(int testNumber);

int main(void)
{	
    int testNumber = 1;
    
    std::cout << "\n*************** TimerTracker tests ***************" << std::endl;
	
    testNumber = TestPart1(testNumber);

    std::cout << "******************************************************\n" << std::endl;
    
    return (0);
}