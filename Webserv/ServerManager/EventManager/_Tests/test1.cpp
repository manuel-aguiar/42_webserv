

// Project headers
# include "../EventManager/EventManager.hpp"
# include "../../../Globals/Globals.hpp"
# include "../../../Event/Event.hpp"
# include "../../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../../../Toolkit/_Tests/test.h"

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

	static void onReadFibonacci(Callback& cb)
	{
		assert(cb.getData() != NULL);

		CalcFibo* calcfibo = reinterpret_cast<CalcFibo*>(cb.getData());
		calcfibo->result = fibonacci(calcfibo->number);
	}

	unsigned int	number;
	long			result;
};

struct WriteHello
{
	WriteHello(int fd) : m_fd(fd) {}

	static void onWriteHello(Callback& cb)
	{
		WriteHello* writeHello = reinterpret_cast<WriteHello*>(cb.getData());
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

		EventManager manager(globals);
		
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	/************************************************** */

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		EventManager	manager(globals);

		int sockfd[2];

		// create a tcp socket pair for communication
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) != 0)
			throw std::runtime_error("Failed to create socket pair" + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		//reader
		Event			readEvent;
		CalcFibo		readCalculate(5);
		long			readExpectedResult = fibonacci(5);

		//writer
		Event			writeEvent;
		WriteHello		writeHello(sockfd[1]);

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
		readEvent.setFdFlagsCallback(sockfd[0], EPOLLIN | EPOLLET, &readCalculate, &CalcFibo::onReadFibonacci);
		FileDescriptor
::setCloseOnExec_NonBlocking(readEvent.getFd());
		manager.addEvent(readEvent);


		writeEvent.setFdFlagsCallback(sockfd[1], EPOLLOUT | EPOLLET, &writeHello, &WriteHello::onWriteHello);
		FileDescriptor
::setCloseOnExec_NonBlocking(writeEvent.getFd());
		manager.addEvent(writeEvent);

		// while we don't have a result, wait and indefinitely until there are events, and process them
		while (readCalculate.result == 0)
			manager.ProcessEvents(-1);

		if (readCalculate.result != readExpectedResult)
			throw std::runtime_error("Events did not get triggered correctly, got fibo" + to_string(readCalculate.result) + ", expected fibo" + to_string(readExpectedResult) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		
		// remove the write event from monitoring, which should lower the subscribe count
		manager.delEvent(writeEvent);

		if (manager.getSubscribeCount() != 1)
			throw std::runtime_error("Events were not deleted correctly, got left " + to_string(manager.getSubscribeCount())  + ", expected: " + to_string(1) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));		

		// modify the read event to actually monitor write, for test purposes, reset result
		// write should be available write away

		readCalculate.result = 0;
		readEvent.setMonitoredFlags(EPOLLOUT | EPOLLET);
		manager.modEvent(readEvent);

		int waitCount = manager.ProcessEvents(-1);

		if (waitCount != 1)
			throw std::runtime_error("waitCount got " + to_string(waitCount) + ", expected " + to_string(1) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (readCalculate.result != readExpectedResult)
			throw std::runtime_error("Events did not get triggered correctly, got fibo" + to_string(readCalculate.result) + ", expected fibo" + to_string(readExpectedResult) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		manager.delEvent(readEvent);

		if (manager.getSubscribeCount() != 0)
			throw std::runtime_error("Events were not deleted correctly got left " + to_string(manager.getSubscribeCount())  + ", expected: " + to_string(0) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));	


		// close the socketpair
		close(sockfd[0]);
		close(sockfd[1]);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	return (testNumber);
}

