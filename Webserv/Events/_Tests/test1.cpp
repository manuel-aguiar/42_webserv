
// testing targets
# include "../Manager/Manager.hpp"
# include "../Subscription/Subscription.hpp"

// Project headers
# include "../../Globals/Globals.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"

// Test helpers
# include "../../GenericUtils/WsTestHelpers/WsTestHelpers.h"

// C++ headers
# include <iostream>

class Calculator
{
	public:
		void Calculate()
		{
			m_data = 42;
		}

		static void EventCallback_Calculate(Events::Subscription& event)
		{
			Calculator* me = reinterpret_cast<Calculator*>(event.accessUser());
			me->Calculate();
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

		Events::Subscription	subscription;

		TestHelpers::assertEqual(subscription.getFd(), -1, "fd not correctly initialized", __FILE__, __LINE__, __FUNCTION__);
		TestHelpers::assertEqual(subscription.getMonitoredEvents(), (Events::Monitor::Mask)Events::Monitor::NONE, "Monitored events not correctly initialized", __FILE__, __LINE__, __FUNCTION__);
		TestHelpers::assertEqual(subscription.getTriggeredEvents(), (Events::Monitor::Mask)Events::Monitor::NONE, "Triggered events not correctly initialized", __FILE__, __LINE__, __FUNCTION__);
		TestHelpers::assertEqual(subscription.accessUser(), (Events::Subscription::User)NULL, "User not correctly initialized", __FILE__, __LINE__, __FUNCTION__);
		TestHelpers::assertEqual(subscription.accessCallback(), (Events::Subscription::Callback)NULL, "Callback not correctly initialized", __FILE__, __LINE__, __FUNCTION__);
		
		std::cout << "	PASSED (Subscription instantiation)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
	
//////////////////////////////////////////////////////////////////////////////////////

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Events::Subscription	subscription;

		subscription.setFd(STDOUT_FILENO);
		TestHelpers::assertEqual(subscription.getFd(), STDOUT_FILENO, "fd not correctly set", __FILE__, __LINE__, __FUNCTION__);
		
		subscription.setMonitoredEvents(Events::Monitor::WRITE);
		TestHelpers::assertEqual(subscription.getMonitoredEvents(), (Events::Monitor::Mask)Events::Monitor::WRITE, "Monitored events not correctly set", __FILE__, __LINE__, __FUNCTION__);
		TestHelpers::assertEqual(subscription.getTriggeredEvents(), (Events::Monitor::Mask)Events::Monitor::NONE, "Triggered events not correctly initialized", __FILE__, __LINE__, __FUNCTION__);
		TestHelpers::assertEqual(subscription.isSubscribed(), false, "Subscription should not be subscribed", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED (Subscription setters/getters)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
//////////////////////////////////////////////////////////////////////////////////////

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Events::Manager 		manager(1, globals);
		Events::Subscription* 	subscription = manager.acquireSubscription();

		Calculator 				calculator;

		subscription->setUser(&calculator);
		subscription->setCallback(Calculator::EventCallback_Calculate);
		subscription->notify();

		TestHelpers::assertEqual(calculator.getData(), 42, "Failed to call the user function", __FILE__, __LINE__, __FUNCTION__);
		
		std::cout << "	PASSED (using a Subscription)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
///////////////////////////////////////////////////////
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Events::Manager 		manager(1, globals);
		Events::Subscription* 	subscription = manager.acquireSubscription();

		Calculator 				calculator;

		subscription->setUser(&calculator);
		subscription->setCallback(Calculator::EventCallback_Calculate);
		// manager.startMonitoring(*subscription, false);  <- correctly asserts, fd is -1;

		subscription->setFd(42);
		// manager.startMonitoring(*subscription, false); <- correctly asserts, no events to monitor here;

		subscription->setMonitoredEvents(Events::Monitor::WRITE);
		// manager.startMonitoring(*subscription, false); <- correctly asserts, fd 42 is not open, EPOLL_CTL fails and aborts

		subscription->setFd(STDOUT_FILENO);
		manager.startMonitoring(*subscription, false); // passes, stdcout is open

		manager.ProcessEvents(-1);

		TestHelpers::assertEqual(calculator.getData(), 42, "Failed to call the user function", __FILE__, __LINE__, __FUNCTION__);
		
		std::cout << "	PASSED (using a Subscription)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

///////////////////////////////////////////////////////

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
		subscription->notify();

		int bytesRead = ::read(testpipe[0], buffer, sizeof(buffer) - 1);
		buffer[bytesRead] = '\0';

		// restore the original stdout
		dup2(stdcerrDup, STDOUT_FILENO);
		close(stdcerrDup);
		close(testpipe[1]);
		close(testpipe[0]);

		TestHelpers::assertEqual(std::string(buffer), std::string("Hello World!"), "Failed to call the user function", __FILE__, __LINE__, __FUNCTION__);
		
		std::cout << "	PASSED (using a Subscription that has no User, just a callback)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	return (testNumber);
}

