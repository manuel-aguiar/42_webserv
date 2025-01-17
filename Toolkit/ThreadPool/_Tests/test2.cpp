

// UnderTest
# include "../ThreadPool.hpp"

// C++ headers
# include <iostream>
# include <unistd.h>
# include <cstring>
# include <sstream>
# include <cstdlib>
# include <vector>

// Test helpers
# include "../../_Tests/ToolkitDummy.hpp"
# include "../../_Tests/ToolkitBase.hpp"
# include "../../_Tests/ToolkitDerived.hpp"
# include "../../_Tests/test.h"

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

int StressTest(int testNumber)
{
/************************************************************** */
	std::cout << "TEST " << testNumber << ": ";
	try
	{
		const int numberOfTasks = 100;

		ThreadPool<100, 1000> tp(20);
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
			tp.addTask(tasks[i]);
		}
			
		tp.waitForCompletion();

		for (size_t i = 0; i < fiboExpected.size(); ++i)
		{
			if (fiboExpected[i] != fiboPlaceResult[i])
				throw std::runtime_error("Didn't calculate fibonacci right");
		}

		std::cout << "	PASSED" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
	testNumber++;	
	return (testNumber);
}





