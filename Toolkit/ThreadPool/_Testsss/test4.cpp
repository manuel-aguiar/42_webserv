

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
# include <vector>

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

int TestPart4(int testNumber)
{
/************************************************************** */
	std::cout << "TEST " << testNumber++ << ": ";

	// Simulating a TaskQueue congestion and users waiting for a slot to execute: addTask(waitForSlot = true)

	try
	{
		const int numberOfTasks = 100;

		//task queue of just 10 slots
		ThreadPool<10, 10> tp(10);
		std::vector<long> 			fiboExpected;
		std::vector<long>			fiboPlaceResult;
		std::vector<FiboTask> 		tasks;

		fiboExpected.reserve(numberOfTasks);
		fiboPlaceResult.reserve(numberOfTasks);
		tasks.reserve(numberOfTasks);

		for (size_t i = 0; i < numberOfTasks; ++i)
		{
			fiboExpected.push_back(fibGood(i));
			tasks.push_back(FiboTask(i, &fiboPlaceResult[i]));
			tp.addTask(tasks[i], true);
		}
			
		tp.waitForCompletion();

		for (size_t i = 0; i < fiboExpected.size(); ++i)
		{
			EXPECT_EQUAL(fiboExpected[i], fiboPlaceResult[i], "Fibonacci value mismatch");
		}

		std::cout << "	PASSED" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
	return (testNumber);
}





