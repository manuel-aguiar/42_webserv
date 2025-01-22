

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
		write(writeHello->m_fd, "Hello", 5);
	}

	int	m_fd;
};

int TestPart1(int testNumber)
{
	/***************************************** */
	Globals			globals(NULL, NULL, NULL, NULL);

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Events::Manager manager(100, globals);
		
		std::cout << "	PASSED (instantiation test)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	/************************************************** */

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Events::Manager	manager(100, globals);

		int sockfd[2];

		// create a unix socket pair for communication
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) != 0)
			throw std::runtime_error("Failed to create socket pair" + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		// set the sockets to non-blocking
		if (!FileDescriptor::setNonBlocking(sockfd[0]) 
		|| !FileDescriptor::setNonBlocking(sockfd[1]))
			throw std::runtime_error("Fcntl failed" + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

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



		// preparing the write event
		readEvent->setFd(sockfd[0]);
		readEvent->setMonitoredFlags(Events::Monitor::READ | Events::Monitor::EDGE_TRIGGERED);
		readEvent->setUser(&readCalculate);
		readEvent->setService(&CalcFibo::onReadFibonacci);

		// subcribe [false] we are subscribing from main and not an event handler, safe to not mark as stale
		manager.add(*readEvent, false);

		writeEvent->setFd(sockfd[1]);
		writeEvent->setMonitoredFlags(Events::Monitor::WRITE | Events::Monitor::EDGE_TRIGGERED);
		writeEvent->setUser(&writeHello);
		writeEvent->setService(&WriteHello::onWriteHello);

		// subcribe [false] we are subscribing from main and not an event handler, safe to not mark as stale
		manager.add(*writeEvent, false);

		// while we don't have a result, wait and indefinitely until there are events, and process them
		while (readCalculate.result == 0)
			manager.ProcessEvents(-1);

		if (readCalculate.result != readExpectedResult)
			throw std::runtime_error("Events did not get triggered correctly, got fibo" + TestHelpers::to_string(readCalculate.result) + ", expected fibo" + TestHelpers::to_string(readExpectedResult) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		
		// remove the write event from monitoring, which should lower the subscribe count
		manager.remove(*writeEvent, false);

		// there should only 1 subscribed
		if (manager.getMonitoringCount() != 1)
			throw std::runtime_error("Events were not deleted correctly, got left " + TestHelpers::to_string(manager.getMonitoringCount())  + ", expected: " + TestHelpers::to_string(1) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));		

		// modify the read event to actually monitor write, for test purposes, reset result
		// write should be available write away

		readCalculate.result = 0;

		// setting read to write because.... we can
		readEvent->setMonitoredFlags(Events::Monitor::WRITE | Events::Monitor::EDGE_TRIGGERED);
		manager.modify(*readEvent, false);

		int waitCount = manager.ProcessEvents(-1);

		if (waitCount != 1)
			throw std::runtime_error("waitCount got " + TestHelpers::to_string(waitCount) + ", expected " + TestHelpers::to_string(1) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (readCalculate.result != readExpectedResult)
			throw std::runtime_error("Events did not get triggered correctly, got fibo" + TestHelpers::to_string(readCalculate.result) + ", expected fibo" + TestHelpers::to_string(readExpectedResult) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		manager.remove(*readEvent, false);

		// should be 0 subcribed
		if (manager.getMonitoringCount() != 0)
			throw std::runtime_error("Events were not deleted correctly got left " + TestHelpers::to_string(manager.getMonitoringCount())  + ", expected: " + TestHelpers::to_string(0) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));	


		// close the socketpair
		close(sockfd[0]);
		close(sockfd[1]);

		std::cout << "	PASSED (handling communication between a writer and a reader)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	return (testNumber);
}

