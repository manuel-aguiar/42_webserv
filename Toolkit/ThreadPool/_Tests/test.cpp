/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   testMain.cpp									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/27 08:33:11 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/09/03 16:26:42 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

# include <iostream>
# include "../ThreadPool/ThreadPool.hpp"


# include <unistd.h>
# include <cstring>
# include <sstream>
# include <cstdlib>

# include "../../MemoryPool/MemoryPool.h"
# include "../../_Tests/ToolkitDummy.hpp"
# include "../../_Tests/ToolkitBase.hpp"
# include "../../_Tests/ToolkitDerived.hpp"
# include "../../Arrays/StackArray/StackArray.hpp"
# include "../../_Tests/test.h"

template <typename T>
std::string to_string(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

long fibGood(unsigned int n)
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
		FiboTask() : m_n(0), m_result(NULL) {}
		FiboTask(unsigned int n, long* result) : m_n(n), m_result(result) {}
		void execute()
		{
			*m_result = fibGood(m_n);
		}
	private:
		unsigned int 	m_n;
		long* 			m_result;
};


int	TestPart1(int testNumber)
{
	std::cout << "TEST " << testNumber << ": ";
	try
	{
		ThreadPool tp(10, 100);
		std::cout << "	PASSED" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
	testNumber++;

/************************************************************** */
	std::cout << "TEST " << testNumber << ": ";
	try
	{
		ThreadPool tp(10, 100);
		tp.removeThread();
		tp.addThread();

		std::cout << "	PASSED" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
	testNumber++;


/************************************************************** */
	std::cout << "TEST " << testNumber << ": ";
	try
	{
		ThreadPool tp(10, 100);
		tp.removeThread();
		tp.removeThread();
		tp.removeThread();

		if (tp.getThreadCount() != 7)
			throw std::runtime_error("Thread count is not 7");

		tp.destroy(true);

		if (tp.getThreadCount() != 0)
			throw std::runtime_error("Thread count is not 0");

		long result;
		FiboTask task(6, &result);

		tp.addTask(task);


		tp.addThread();
		tp.waitForCompletion();
		tp.removeThread();

		if (result != 8)
			throw std::runtime_error("Didn't calculate fibonacci right");

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

int TestPart2(int testNumber)
{
/************************************************************** */
	std::cout << "TEST " << testNumber << ": ";
	try
	{

		ThreadPool 					tp(10, 20);
		StackArray<long, 100> 		fiboExpected;
		StackArray<long, 100>		fiboPool;
		StackArray<FiboTask, 100> 	tasks;

		for (size_t i = 0; i < fiboPool.size(); ++i)
		{
			fiboExpected.emplace_back(fibGood(i));
			tasks.emplace_back(i, &fiboPool[i]);
			tp.addTask(tasks[i]);
		}
			
		tp.waitForCompletion();

		for (size_t i = 0; i < fiboExpected.size(); ++i)
		{
			if (fiboExpected[i] != fiboPool[i])
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

int main(void)
{

    int testNumber = 1;
	/******************* TEST 1 ************************/
	std::cout << "\n************ ThreadPool tests *************" << std::endl;
	
    testNumber = TestPart1(testNumber);
	testNumber = TestPart2(testNumber);

	std::cout << "*******************************************\n" << std::endl;
	
	return (0);
}





