

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

// C headers
# include <unistd.h>

static long fibonacci(unsigned int n)
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

struct CalcFibo
{
	CalcFibo(unsigned int n) : number(n), result(0) {}

	static void onReadFibonacci(Events::Subscription& cb)
	{
		CalcFibo* calcfibo = reinterpret_cast<CalcFibo*>(cb.accessUser());
		calcfibo->result = fibonacci(calcfibo->number);
	}

	unsigned int	number;
	long			result;
};

struct WriteHello
{
	WriteHello(int fd) : m_fd(fd) {}

	static void onWriteHello(Events::Subscription& cb)
	{
		WriteHello* writeHello = reinterpret_cast<WriteHello*>(cb.accessUser());
		::write(writeHello->m_fd, "Hello", 5);
	}

	int	m_fd;
};

int TestPart2(int testNumber)
{
	/***************************************** */
	Globals			globals(NULL, NULL, NULL, NULL);

	try
	{
		TEST_INTRO(testNumber++);

		Events::Manager manager(100, globals);
		
		TEST_PASSED_MSG("Events::Manager instantiation");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/************************************************** */

	try
	{
		TEST_INTRO(testNumber++);

		Events::Manager	manager(100, globals);

		int sockfd[2];

		// create a unix socket pair for communication, just check all went well
		EXPECT_EQUAL(socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd), 0, "Failed to create socket pair");
		EXPECT_EQUAL(FileDescriptor::setNonBlocking(sockfd[0]) == 0, false, "Fcntl failed");
		EXPECT_EQUAL(FileDescriptor::setNonBlocking(sockfd[1]) == 0, false, "Fcntl failed");

		//reader
		Events::Subscription*			readEvent = manager.acquireSubscription();
		CalcFibo						readCalculate(5);
		long							readExpectedResult = fibonacci(5);

		//writer
		Events::Subscription*			writeEvent = manager.acquireSubscription();
		WriteHello						writeHello(sockfd[1]);

		/*
			WriteHello will write "Hello" on the write on its socket, when the event manager says its socket
			can be written to.

			readCalculate will calculate the 5th fibonacci number when the event manager says its socket
			can be read from.

			Subscribed edge-triggered because i won't be reading the sockets and don't want the event manager
			to keep triggering the event.

			It is expected the write event is triggered right away. WriteHello writes and triggers the readEvent.
			On the next epoll iteration, the readEvent is received, calculating fibonacci.
		*/



		// preparing the read event
		readEvent->setFd(sockfd[0]);
		readEvent->setMonitoredEvents(Events::Monitor::READ | Events::Monitor::EDGE_TRIGGERED);
		readEvent->setUser(&readCalculate);
		readEvent->setCallback(&CalcFibo::onReadFibonacci);

		// subcribe [false] we are subscribing from main and not an event handler, safe to not mark as stale
		manager.startMonitoring(*readEvent, false);

		writeEvent->setFd(sockfd[1]);
		writeEvent->setMonitoredEvents(Events::Monitor::WRITE | Events::Monitor::EDGE_TRIGGERED);
		writeEvent->setUser(&writeHello);
		writeEvent->setCallback(&WriteHello::onWriteHello);

		// subcribe [false] we are subscribing from main and not an event handler, safe to not mark as stale
		manager.startMonitoring(*writeEvent, false);

		// while we don't have a result, wait and indefinitely until there are events, and process them
		while (readCalculate.result == 0)
			manager.ProcessEvents(-1);

		EXPECT_EQUAL(readCalculate.result, readExpectedResult, "Failed to calculate fibonacci");
		
		manager.stopMonitoring(*writeEvent, false);

		// there should only 1 subscribed
		EXPECT_EQUAL(manager.getMonitoringCount(), 1, "Events were not deleted correctly");

		// modify the read event to actually monitor write, for test purposes, reset result
		// write should be available write away

		readCalculate.result = 0;

		// setting read to write because.... we can
		readEvent->setMonitoredEvents(Events::Monitor::WRITE | Events::Monitor::EDGE_TRIGGERED);
		manager.updateEvents(*readEvent, false);

		int waitCount = manager.ProcessEvents(-1);
		EXPECT_EQUAL(waitCount, 1, "Events did not get triggered correctly");
		EXPECT_EQUAL(readCalculate.result, readExpectedResult, "Events did not get triggered correctly");

		manager.stopMonitoring(*readEvent, false);

		// should be 0 subcribed
		EXPECT_EQUAL(manager.getMonitoringCount(), 0, "Events were not deleted correctly");


		// close the socketpair
		::close(sockfd[0]);
		::close(sockfd[1]);

		TEST_PASSED_MSG("handling communication between a writer and a reader");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	return (testNumber);
}

