
// target
# include "../SignalHandler.hpp"

//helpers
# include "../../../Toolkit/TestHelpers/TestHelpers.h"

//dependencies
# include "../../Globals/Globals.hpp"

SignalHandler 	g_SignalHandler;

int main(void)
{
	int testNumber = 1;

	TEST_HEADER("SignalHandler");


	int ourPid = ::getpid();
	try
	{
		TEST_INTRO(testNumber++);

		//SignalHandler handler; <- correctly asserts, SignalHandler is a global and cannot be instantiated outside global scope
		//SignalHandler copy(g_SignalHandler); <- private
		TEST_PASSED_MSG("SignalHandler instantiation");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	try
	{
		TEST_INTRO(testNumber++);

		Globals globals(NULL, NULL, NULL, NULL);
		g_SignalHandler.openSignalListeners(1, globals);

		Ws::fd listener = g_SignalHandler.getSignalListener(0);

		g_SignalHandler.registerSignal(SIGINT, globals);

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

		TEST_PASSED_MSG("sending a signal and confirming all good");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	try
	{
		TEST_INTRO(testNumber++);

		const int numServers = 100;

		Globals globals(NULL, NULL, NULL, NULL);
		g_SignalHandler.openSignalListeners(numServers, globals);
		DynArray<Ws::fd> listeners(numServers);

		for (size_t i = 0; i < listeners.size(); ++i)
			listeners[i] = g_SignalHandler.getSignalListener(i);

		g_SignalHandler.registerSignal(SIGINT, globals);
		g_SignalHandler.registerSignal(SIGQUIT, globals);

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

	try
	{
		TEST_INTRO(testNumber++);

		const int numServers = 1;

		Globals globals(NULL, NULL, NULL, NULL);
		g_SignalHandler.openSignalListeners(numServers, globals);

		// ignoring
		g_SignalHandler.ignoreSignal(SIGINT, globals);
		g_SignalHandler.ignoreSignal(SIGQUIT, globals);

		// sending
		::kill(ourPid, SIGINT);
		::kill(ourPid, SIGQUIT);
		::kill(ourPid, SIGINT);
		::kill(ourPid, SIGQUIT);

		g_SignalHandler.registerSignal(SIGINT, globals);
		g_SignalHandler.registerSignal(SIGQUIT, globals);

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

	TEST_FOOTER;
	return (0);
}

