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
# include "../ThreadPool.h"


# include <unistd.h>
# include <cstring>
# include <sstream>
# include <cstdlib>
# include <vector>

# include "../../_Tests/ToolkitDummy.hpp"
# include "../../_Tests/ToolkitBase.hpp"
# include "../../_Tests/ToolkitDerived.hpp"
# include "../../_Tests/test.h"

template <typename T>
std::string to_string(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

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

		long placeResult;
		FiboTask task(6, &placeResult);

		tp.addTask(task);


		tp.addThread();
		tp.waitForCompletion();
		tp.removeThread();

		if (placeResult != 8)
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






