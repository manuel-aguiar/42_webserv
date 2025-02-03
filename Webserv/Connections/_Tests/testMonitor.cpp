
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

void testMonitor(int& testNumber)
{
    Globals globals(NULL, NULL, NULL, NULL);
    ////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);
        Monitor monitor;
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

        Events::Manager eventManager(1, globals);
        Monitor monitor;

        //Events::Subscription& subs = monitor.accessEvent(); <- correctly asserts, trying to access event before having subscribed one
        
        monitor.acquire(eventManager);
        //monitor.acquire(eventManager); <- correctly asserts, already acquired

        //monitor.subscribe(eventManager, true); <- event manager asserts, fd = FD_NONE

        Events::Subscription& subs = monitor.accessEvent();

        subs.setFd(STDIN_FILENO);
        subs.setMonitoredEvents(Events::Monitor::READ | Events::Monitor::ERROR | Events::Monitor::HANGUP | Events::Monitor::EDGE_TRIGGERED);

        monitor.subscribe(eventManager, true);

        subs.setMonitoredEvents(Events::Monitor::WRITE);
        //monitor.subscribe(eventManager, true); <- correctly asserts, already subscribed

        monitor.modify(eventManager, true);

        // successfully asserts in case the monitor doesn't return the subscription
        monitor.release(eventManager);

        EXPECT_EQUAL(eventManager.getMonitoringCount(), 0, "Monitor::release() should have removed the subscription from the event manager");

        TEST_PASSED_MSG("Monitor: acquire/release");
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
    ////////////////////////////////////////////
}