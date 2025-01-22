

// Project headers
# include "../Events.h"
# include "../../Globals/Globals.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"

// Test helpers
# include "../../../Toolkit/_Tests/TestHelpers.h"

// C++ headers
# include <iostream>

class Calculator
{
	public:
		void doSomething()
		{
			m_data = 42;
		}

		static void MyCallback_doSomething(Events::Subscription& event)
		{
			Calculator* me = reinterpret_cast<Calculator*>(event.accessUser());
			me->doSomething();
		}

		int getData() const
		{
			return (m_data);
		}
	private:
		int	m_data;
};

void	PrintHelloWorld(Events::Subscription& callback)
{
	(void)callback;

	std::cout << "Hello World!";
	std::cout.flush();
}

int TestPart1(int testNumber)
{
	/***************************************** */
	Globals			globals(NULL, NULL, NULL, NULL);

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Events::Manager manager(100, globals);
		Calculator 		calculator;
		Events::Subscription* subscription = manager.acquireSubscription();

		subscription->setUser(&calculator);
		subscription->setCallback(Calculator::MyCallback_doSomething);

		subscription->notifyUser();

		if (calculator.getData() != 42)
			throw std::runtime_error("Failed to call the user function" + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		
		std::cout << "	PASSED (using a Subscription)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::cout.flush();

		// prepare some pipes
		int testpipe[2];
		int stdcerrDup = dup(STDOUT_FILENO);
		pipe(testpipe);
		dup2(testpipe[1], STDOUT_FILENO);
		char buffer[64];

		Events::Manager manager(100, globals);
		Events::Subscription* subscription = manager.acquireSubscription();

		subscription->setCallback(&PrintHelloWorld);
		subscription->notifyUser();

		int bytesRead = ::read(testpipe[0], buffer, sizeof(buffer) - 1);
		buffer[bytesRead] = '\0';

		// restore the original stdout
		dup2(stdcerrDup, STDOUT_FILENO);
		close(stdcerrDup);
		close(testpipe[1]);
		close(testpipe[0]);

		if (std::string(buffer) != "Hello World!")
			throw std::runtime_error("Failed to call the user function" + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		
		std::cout << "	PASSED (using a Subscription that has no User, just a callback)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	return (testNumber);
}

