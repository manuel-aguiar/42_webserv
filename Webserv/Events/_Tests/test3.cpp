

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
		Calculator() : m_data(0) {}

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

int TestPart3(int testNumber)
{
	/***************************************** */
	Globals			globals(NULL, NULL, NULL, NULL);

	try
	{
		std::cout << TEST_CLR_BLUE << "TEST " << testNumber++ << ": " << TEST_CLR_RESET;

		Events::Manager 		manager(100, globals);
		Events::Subscription* 	subscription = manager.acquireSubscription();
		Calculator				user;

		subscription->setUser(&user);
		subscription->setCallback(&Calculator::MyCallback_doSomething);

		// let me know when i can write to stdout
		subscription->setFd(STDOUT_FILENO);
		subscription->setMonitoredEvents(Events::Monitor::WRITE);

		manager.startMonitoring(*subscription, true);	//event added as stale, if triggered do not execute

		//stdout should be ready straight away
		manager.ProcessEvents(-1); 

		// data should be 0, triggered but not handled because event fd was market as stale
		TestHelpers::assertEqual(user.getData(), 0, "Events should not be handled, marked as stale", __FILE__, __LINE__, __FUNCTION__);
		
		// after processing events, all fds are marked as good again, until proven otherwise
		manager.ProcessEvents(-1);

		// handler should be called now, and user data should be 42
		TestHelpers::assertEqual(user.getData(), 42, "Events should now be handled, not stale anymore", __FILE__, __LINE__, __FUNCTION__);

		 //stopMonitoring event, (indifferent to mark as stale here)
		manager.stopMonitoring(*subscription, true);

		manager.returnSubscription(*subscription);

		std::cout << "	PASSED " << "(stale event test)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << TEST_CLR_RED << "	FAILED: " << TEST_CLR_RESET << e.what()  << std::endl;
	}

	/************************************************** */


	return (testNumber);
}

