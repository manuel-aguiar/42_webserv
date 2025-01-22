

// Project headers
# include "../Events.h"
# include "../Manager/Manager.hpp"
# include "../Subscription/Subscription.hpp"
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

int TestPart2(int testNumber)
{
	/***************************************** */
	Globals			globals(NULL, NULL, NULL, NULL);

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Events::Manager 		manager(100, globals);
		Events::Subscription* 	subscription = manager.acquireSubscription();
		Calculator				user;

		subscription->setUser(&user);
		subscription->setService(&Calculator::MyCallback_doSomething);

		// let me know when i can write to stdout
		subscription->setFd(STDOUT_FILENO);
		subscription->setMonitoredFlags(Events::Monitor::WRITE);

		manager.add(*subscription, true);	//event added as stale, if triggered do not execute

		//stdout should be ready straight away
		manager.ProcessEvents(-1); 

		// data should be 0, triggered but not handled because event fd was market as stale
		if (user.getData() != 0)
			throw std::runtime_error("Events should not be handled, marked as stale, got " 
			+ StringUtils::to_string(user.getData()) + ", expected " 
			+ StringUtils::to_string(0) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		
		// after processing events, all fds are marked as good again, until proven otherwise
		manager.ProcessEvents(-1);

		// handler should be called now, and user data should be 42
		if (user.getData() != 42)
			throw std::runtime_error("Events should not be handled, marked as stale, got " 
			+ StringUtils::to_string(user.getData()) + ", expected " 
			+ StringUtils::to_string(42) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		 //remove event, (indifferent to mark as stale here)
		manager.remove(*subscription, true);
		
		manager.returnSubscription(*subscription);

		std::cout << "	PASSED (stale event test)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	/************************************************** */


	return (testNumber);
}

