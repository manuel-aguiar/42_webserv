
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
		TEST_INTRO(testNumber++);

		Events::Subscription	subscription;

		EXPECT_EQUAL(subscription.getFd(), -1, "fd not correctly initialized");
		EXPECT_EQUAL(subscription.getMonitoredEvents(), Events::Monitor::NONE, "Monitored events not correctly initialized");
		EXPECT_EQUAL(subscription.getTriggeredEvents(), Events::Monitor::NONE, "Triggered events not correctly initialized");
		EXPECT_EQUAL(subscription.accessUser(), (Events::Subscription::User)NULL, "User not correctly initialized");
		EXPECT_EQUAL(subscription.accessCallback(), (Events::Subscription::Callback)NULL, "Callback not correctly initialized");
		
		std::cout << "	PASSED (Subscription instantiation)" << std::endl;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

//////////////////////////////////////////////////////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);

		Events::Subscription	subscription;

		subscription.setFd(STDOUT_FILENO);
		EXPECT_EQUAL(subscription.getFd(), STDOUT_FILENO, "fd not correctly set");
		
		subscription.setMonitoredEvents(Events::Monitor::WRITE);
		EXPECT_EQUAL(subscription.getMonitoredEvents(), (Events::Monitor::Mask)Events::Monitor::WRITE, "Monitored events not correctly set");
		EXPECT_EQUAL(subscription.getTriggeredEvents(), (Events::Monitor::Mask)Events::Monitor::NONE, "Triggered events not correctly initialized");
		EXPECT_EQUAL(subscription.isSubscribed(), false, "Subscription should not be subscribed");

		std::cout << "	PASSED (Subscription setters/getters)" << std::endl;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

//////////////////////////////////////////////////////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);

		Events::Subscription	subscription;

		//subscription.setFd(-42);  <- correctly asserts, fd must be bigger than -1

		//subscription.setMonitoredEvents(-1); <- correctly asserts, this is not a valid flag combination

		std::cout << "	PASSED (Subscription internal asserts)" << std::endl;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
//////////////////////////////////////////////////////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);

		Calculator 				calculator;
		Events::Subscription	subscription;

		subscription.setUser(&calculator);
		subscription.setCallback(Calculator::EventCallback_Calculate);

		EXPECT_EQUAL(subscription.accessUser(), (Events::Subscription::User)&calculator, "User not correctly set");
		EXPECT_EQUAL(subscription.accessCallback(), (Events::Subscription::Callback)Calculator::EventCallback_Calculate, "Callback not correctly set");
		
		subscription.notify();

		EXPECT_EQUAL(calculator.getData(), 42, "Failed to call the user function");
		
		std::cout << "	PASSED (using a Subscription)" << std::endl;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
///////////////////////////////////////////////////////
	try
	{
		TEST_INTRO(testNumber++);

		Events::Manager 		manager(1, globals);
		Events::Subscription* 	subscription = manager.acquireSubscription();

		Calculator 				calculator;

		subscription->setUser(&calculator);
		subscription->setCallback(Calculator::EventCallback_Calculate);
		// manager.startMonitoring(*subscription, false);  <- correctly asserts, fd is -1;

		subscription->setFd(42);
		// manager.startMonitoring(*subscription, false); <- correctly asserts, no events to monitor here;

		subscription->setMonitoredEvents(Events::Monitor::READ | Events::Monitor::HANGUP | Events::Monitor::ERROR);
		// manager.startMonitoring(*subscription, false); <- correctly asserts, fd 42 is not open, EPOLL_CTL fails and aborts

		subscription->setFd(STDOUT_FILENO);

				EXPECT_EQUAL(subscription->isSubscribed(), false, "Subscription should not be subscribed");

		//manager.stopMonitoring(*subscription, false); <- correctly asserts, subscription is not being monitored so it can't be unsubscribed

		manager.startMonitoring(*subscription, false); // passes, stdcout is open

				EXPECT_EQUAL(subscription->isSubscribed(), true, "Subscription should be subscribed");

				EXPECT_EQUAL(subscription->getTriggeredEvents(), (Events::Monitor::Mask)Events::Monitor::NONE, 
					"There should be no triggered events, manager.processevents was not yet called");

				EXPECT_EQUAL(manager.getMonitoringCount(), 1, "Manager Failed to subscribe");

		manager.ProcessEvents(10);

				EXPECT_EQUAL(calculator.getData(), -1, "Should not have triggered any event, STDOUT is not available for reading");

		subscription->setMonitoredEvents(Events::Monitor::WRITE | Events::Monitor::HANGUP | Events::Monitor::ERROR);
	
		manager.ProcessEvents(10);
				EXPECT_EQUAL(calculator.getData(), -1, "Should not have triggered any event, \
			Subscription now tracks writing but the event manager was not informed");

		manager.updateEvents(*subscription, false);
		manager.updateEvents(*subscription, false);
		manager.updateEvents(*subscription, false);
		manager.updateEvents(*subscription, false);

		manager.ProcessEvents(-1);

		// should only trigger the write event
				EXPECT_EQUAL(subscription->getTriggeredEvents(), (Events::Monitor::Mask)Events::Monitor::WRITE, "Failed to trigger the event");
				EXPECT_EQUAL(calculator.getData(), 42, "Failed to call the user function");
		
		//manager.startMonitoring(*subscription, false);  <- correctly asserts, subscription is already being monitored

		manager.stopMonitoring(*subscription, false); //works fine

				EXPECT_EQUAL(subscription->isSubscribed(), false, "Subscription should not be subscribed");
				EXPECT_EQUAL(manager.getMonitoringCount(), 0, "Failed to unsubscribe");

		//manager.updateEvents(*subscription, false);  <- correctly asserts, subscription is not being monitored, cannot be changed

		manager.ProcessEvents(-1);
		manager.ProcessEvents(-1);
		manager.ProcessEvents(-1);
		manager.ProcessEvents(-1);
		manager.ProcessEvents(-1);
		// returns immediatelly, no subscribers

		std::cout << "	PASSED (Subscription with EventManager)" << std::endl;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

///////////////////////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);
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

		EXPECT_EQUAL(std::string(buffer), std::string("Hello World!"), "Failed to call the user function");
		
		std::cout << "	PASSED (using a Subscription that has no User, just a callback)" << std::endl;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	return (testNumber);
}

