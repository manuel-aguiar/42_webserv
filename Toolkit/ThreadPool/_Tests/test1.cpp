

//test target
# include "../ThreadPool.hpp"


// Project headers
# include "../../_Tests/ToolkitDummy.hpp"
# include "../../_Tests/ToolkitBase.hpp"
# include "../../_Tests/ToolkitDerived.hpp"
# include "../../TestHelpers/TestHelpers.h"

// C++ headers
# include <unistd.h>
# include <cstring>
# include <sstream>
# include <cstdlib>
# include <iostream>


static long fibGood(unsigned int n)
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

class FiboTask : public IThreadTask
{
	public:
		FiboTask() : m_sequencePosition(0), m_placeResult(NULL) {}
		FiboTask(unsigned int n, long* placeResult) : m_sequencePosition(n), m_placeResult(placeResult) {}
		void execute()
		{
			*m_placeResult = fibGood(m_sequencePosition);
		}
	private:
		unsigned int 	m_sequencePosition;
		long* 			m_placeResult;
};


int	TestPart1(int testNumber)
{
	std::cout << "TEST " << testNumber << ": ";
	try
	{
		ThreadPool<10, 100> tp(5);
		std::cout << "	PASSED" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
	testNumber++;

/************************************************************** */
	std::cout << "TEST " << testNumber << ": ";
	try
	{
		ThreadPool<100, 1000> tp(10);
		tp.removeThread();
		tp.addThread();

		std::cout << "	PASSED" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
	testNumber++;


/************************************************************** */
	std::cout << "TEST " << testNumber << ": ";
	try
	{
		ThreadPool<100, 1000> tp(10);
		tp.removeThread();
		tp.removeThread();
		tp.removeThread();

		if (tp.getThreadCount() != 7)
			throw std::runtime_error("Thread count is not 7");

		tp.destroy(true);

		if (tp.getThreadCount() != 0)
			throw std::runtime_error("Thread count is not 0");

		long placeResult;
		FiboTask task(6, &placeResult);

		tp.addTask(task);

		tp.addThread();
		tp.waitForCompletion();
		tp.removeThread();

		EXPECT_EQUAL(placeResult, (long)8, "Didn't calculate fibonacci right");

		std::cout << "	PASSED" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
	testNumber++;

	return (testNumber);
}






