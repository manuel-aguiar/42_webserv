
// target
# include "../SignalHandler.hpp"

//helpers
# include "../../../Toolkit/TestHelpers/TestHelpers.h"

//dependencies
# include "../../Globals/Globals.hpp"

// C headers
# include <signal.h>
# include <unistd.h>


int main(void)
{
	int testNumber = 1;

	TEST_HEADER("SignalHandler");

	// common process and common NULL
	int ourPid = ::getpid();

	// reset the signal handler after every test
	g_SignalHandler.reset(NULL);
	try
	{
		TEST_INTRO(testNumber++);

		//SignalHandler handler; <- correctly asserts, SignalHandler is a global and cannot be instantiated outside global scope
		//SignalHandler copy(g_SignalHandler); <- private

		//g_SignalHandler.unregisterSignal(SIGINT, NULL); <- correctly asserts, signal was not registered/ignored

		TEST_PASSED_MSG("SignalHandler instantiation and assert tests");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
///////////////////////////////////////////////////////////////////////////////////////
	// reset the signal handler after every test
	g_SignalHandler.reset(NULL);
	try
	{
		TEST_INTRO(testNumber++);

		//SignalHandler handler; <- correctly asserts, SignalHandler is a global and cannot be instantiated outside global scope
		//SignalHandler copy(g_SignalHandler); <- private

		g_SignalHandler.registerSignal(5, NULL);
		g_SignalHandler.registerSignal(10, NULL);
		g_SignalHandler.registerSignal(15, NULL);
		g_SignalHandler.registerSignal(16, NULL);


		TEST_PASSED_MSG("registering random but legal signals");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
///////////////////////////////////////////////////////////////////////////////////////
	// reset the signal handler after every test
	g_SignalHandler.reset(NULL);

	try
	{
		TEST_INTRO(testNumber++);

		
		g_SignalHandler.openSignalListeners(1, NULL);

		Ws::fd listener = g_SignalHandler.getSignalListener(0);

		g_SignalHandler.registerSignal(SIGINT, NULL);

		// send sigint to ourselves
		::kill(ourPid, SIGINT);
		::kill(ourPid, SIGINT);
		::kill(ourPid, SIGINT);
		::kill(ourPid, SIGINT);
		::kill(ourPid, SIGINT);
		::kill(ourPid, SIGINT);

		char buffer[100];
		int bytesRead = ::read(listener, buffer, sizeof(buffer));

		EXPECT_EQUAL(bytesRead, 6, "SignalHandler should have read 6 bytes from the pipe");
		EXPECT_EQUAL(g_SignalHandler.getSignal(), SIGINT, "Signal handler should have noticed SIGINT");
		EXPECT_EQUAL(buffer[0], SIGINT, "Signal handler should have read SIGINT from the pipe");
		EXPECT_EQUAL(buffer[1], SIGINT, "Signal handler should have read SIGINT from the pipe");
		EXPECT_EQUAL(buffer[2], SIGINT, "Signal handler should have read SIGINT from the pipe");
		EXPECT_EQUAL(buffer[3], SIGINT, "Signal handler should have read SIGINT from the pipe");
		EXPECT_EQUAL(buffer[4], SIGINT, "Signal handler should have read SIGINT from the pipe");
		EXPECT_EQUAL(buffer[5], SIGINT, "Signal handler should have read SIGINT from the pipe");

		TEST_PASSED_MSG("sending a signal and confirming all good");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
///////////////////////////////////////////////////////////////////////////////////////
	// reset the signal handler after every test
	g_SignalHandler.reset(NULL);
	try
	{
		TEST_INTRO(testNumber++);

		const int numServers = 100;

		g_SignalHandler.openSignalListeners(numServers, NULL);
		DynArray<Ws::fd> listeners(numServers);

		for (size_t i = 0; i < listeners.size(); ++i)
			listeners[i] = g_SignalHandler.getSignalListener(i);

		g_SignalHandler.registerSignal(SIGINT, NULL);
		g_SignalHandler.registerSignal(SIGQUIT, NULL);

		// send sigint to ourselves
		::kill(ourPid, SIGINT);
		::kill(ourPid, SIGQUIT);
		::kill(ourPid, SIGINT);
		::kill(ourPid, SIGQUIT);

		// confirming all listeners received them, in order
		for (size_t i = 0; i < listeners.size(); ++i)
		{
			char buffer[100];
			int bytesRead = ::read(listeners[i], buffer, sizeof(buffer));
			EXPECT_EQUAL(bytesRead, 4, TestHelpers::to_string(i) + " server: SignalHandler should have read 4 byte from the pipe");
			EXPECT_EQUAL(buffer[0], SIGINT, TestHelpers::to_string(i) + " server: Signal handler should have read SIGINT from the pipe");
			EXPECT_EQUAL(buffer[1], SIGQUIT, TestHelpers::to_string(i) + " server: Signal handler should have read SIGQUIT from the pipe");
			EXPECT_EQUAL(buffer[2], SIGINT, TestHelpers::to_string(i) + " server: Signal handler should have read SIGINT from the pipe");
			EXPECT_EQUAL(buffer[3], SIGQUIT, TestHelpers::to_string(i) + " server: Signal handler should have read SIGQUIT from the pipe");
		}

		

		TEST_PASSED_MSG("sending signals and reading them from multiple servers");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
///////////////////////////////////////////////////////////////////////////////////////
	// reset the signal handler after every test
	g_SignalHandler.reset(NULL);

	try
	{
		TEST_INTRO(testNumber++);

		const int numServers = 1;

		g_SignalHandler.openSignalListeners(numServers, NULL);

		// ignoring
		g_SignalHandler.ignoreSignal(SIGINT, NULL);
		g_SignalHandler.ignoreSignal(SIGQUIT, NULL);

		// sending
		::kill(ourPid, SIGINT);
		::kill(ourPid, SIGQUIT);
		::kill(ourPid, SIGINT);
		::kill(ourPid, SIGQUIT);

		g_SignalHandler.registerSignal(SIGINT, NULL);
		g_SignalHandler.registerSignal(SIGQUIT, NULL);

		// send again
		::kill(ourPid, SIGINT);
		::kill(ourPid, SIGQUIT);
		::kill(ourPid, SIGINT);
		::kill(ourPid, SIGQUIT);

		char buffer[100];
		int bytesRead = ::read(g_SignalHandler.getSignalListener(0), buffer, sizeof(buffer));
		EXPECT_EQUAL(bytesRead, 4, "SignalHandler should have read only 4 bytes, previous signals were ignored");
		EXPECT_EQUAL(buffer[0], SIGINT, "Signal handler should have read SIGINT from the pipe");
		EXPECT_EQUAL(buffer[1], SIGQUIT, "Signal handler should have read SIGQUIT from the pipe");
		EXPECT_EQUAL(buffer[2], SIGINT, "Signal handler should have read SIGINT from the pipe");
		EXPECT_EQUAL(buffer[3], SIGQUIT, "Signal handler should have read SIGQUIT from the pipe");

		

		TEST_PASSED_MSG("ignoring signals");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
///////////////////////////////////////////////////////////////////////////////////////

	g_SignalHandler.reset(NULL);

	TEST_FOOTER;
	return (0);
}

