

// Test subject
# include "../Event.hpp"

//Project headers
# include "../../../Toolkit/_Tests/test.h"


# include <iostream>

static long fibonacci(unsigned int n)
{
	long a;
	long b;
	long temp;

	if (n <= 1)
		return (n);
	a = 0;
	b = 1;
	while (n > 0)
	{
		temp = b;
		b = a + b;
		a = temp;
		n--;
	}
	return (a);
}

struct CalcFibo
{
	CalcFibo(unsigned int n) : number(n), result(0) {}

	static void executeFibonacci(Callback& cb)
	{
		assert(cb.getData() != NULL);

		CalcFibo* calcfibo = reinterpret_cast<CalcFibo*>(cb.getData());
		calcfibo->result = fibonacci(calcfibo->number);
	}

	unsigned int	number;
	long			result;
};

int main(void)
{
	int testNumber = 1;

	std::cout << "\n*************** Callback tests ***************" << std::endl;

	try
	{
		std::cout << "Test " << testNumber++ << ": ";

		CalcFibo 	calculate(5);
		Event 		event;

		long expectedResult = fibonacci(5);

		// these must be tested in connection to the EventManager
		event.setFd(123);
		event.setMonitoredFlags(EPOLLIN + 123 + 345);

		event.setCallback(&calculate, CalcFibo::executeFibonacci);


		// EventManager identifies the event took place, calls handle:
		event.handle();

		if (calculate.result != expectedResult)
			throw std::runtime_error("Callback failed to execute, got " + to_string(calculate.result) + ", expected " + to_string(expectedResult) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	std::cout << "*********************************************\n" << std::endl;
}