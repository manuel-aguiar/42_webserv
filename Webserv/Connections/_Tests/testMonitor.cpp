
// target
# include "../Monitor/Monitor.hpp"

// dependencies
# include "../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../../Events/Manager/Manager.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../Globals/Globals.hpp"

// C++ headers
#include <cstdlib>
#include <iostream>


// C headers
#include <unistd.h>
#include <arpa/inet.h>

class Calculator
{
	public:

		enum {NOT_CALCULATED = -1, RESULT = 42};

		Calculator() : m_data(NOT_CALCULATED) {}
		void Calculate()
		{
			m_data = RESULT;
		}

		static void EventCallback_Calculate(Events::Subscription& event)
		{
			Calculator* me = reinterpret_cast<Calculator*>(event.accessUser());
			if (event.getTriggeredEvents() == Events::Monitor::NONE
			|| event.getTriggeredEvents() & Events::Monitor::WRITE)
				me->Calculate();
		}

		int getData() const
		{
			return (m_data);
		}
	private:
		int	m_data;
};

void testMonitor(int& testNumber)
{
    Globals globals(NULL, NULL, NULL, NULL);
    Events::Manager eventManager(1, globals);
    ////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);

        Monitor monitor(eventManager);

        monitor.acquire();

        TEST_PASSED_MSG("Monitor: instantiation");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
    ////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);

        Monitor monitor(eventManager);

        //Events::Subscription& subs = monitor.accessEvent(); <- correctly asserts, trying to access event before having subscribed one
        
        monitor.acquire();
        //monitor.acquire(eventManager); <- correctly asserts, already acquired

        //monitor.subscribe(eventManager, true); <- event manager asserts, fd = FD_NONE

        Events::Subscription& subs = monitor.accessEvent();

        subs.setFd(STDIN_FILENO);
        subs.setMonitoredEvents(Events::Monitor::READ | Events::Monitor::ERROR | Events::Monitor::HANGUP | Events::Monitor::EDGE_TRIGGERED);

        monitor.subscribe(true);

        subs.setMonitoredEvents(Events::Monitor::WRITE);
        //monitor.subscribe(eventManager, true); <- correctly asserts, already subscribed

        monitor.modify(true);

        // successfully asserts in case the monitor doesn't return the subscription
        monitor.release();

        EXPECT_EQUAL(eventManager.getMonitoringCount(), 0, "Monitor::release() should have removed the subscription from the event manager");

        TEST_PASSED_MSG("Monitor: acquire/release");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
    ////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);

        Events::Manager eventManager(1, globals);




        Monitor monitor(eventManager);

        monitor.acquire();
        Events::Subscription& subs = monitor.accessEvent();

		Calculator 				calculator;


        subs.setFd(STDIN_FILENO);
        subs.setMonitoredEvents(Events::Monitor::WRITE | Events::Monitor::ERROR | Events::Monitor::HANGUP | Events::Monitor::EDGE_TRIGGERED);
		subs.setUser(&calculator);
		subs.setCallback(Calculator::EventCallback_Calculate);
        
        monitor.subscribe(false);

        eventManager.ProcessEvents(-1);

        // successfully asserts in case the monitor doesn't return the subscription
        monitor.release();

        EXPECT_EQUAL(calculator.getData(), (int)Calculator::RESULT, "Failed to call the user function");

        EXPECT_EQUAL(eventManager.getMonitoringCount(), 0, "Monitor::release() should have removed the subscription from the event manager");

        TEST_PASSED_MSG("Monitor: triggering events");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
    ////////////////////////////////////////////
}