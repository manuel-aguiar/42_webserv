

# include "../EventManager.hpp"
# include "TestDependencies.hpp"
# include <iostream>
# include "../../../Globals/Globals.hpp"
# include "../../../Event/Event.hpp"
# include "../../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

void EventHandler(Event& event)
{
	char buffer[100];
	int readChars;

	std::cout << "Event triggered, reading fd: ";

	while ((readChars = read(event.getFd(), buffer, 100)) > 0)
	{
		buffer[readChars] = '\0';
		std::cout << buffer << std::endl;
	}
		
}

int main(void)
{
	/***************************************** */
	Clock			clock;
	LogFile			statusFile("status.log");
	LogFile			errorFile("error.log");
	LogFile			debugFile("debug.log");
	Globals			globals(&clock, &statusFile, &errorFile, &debugFile);

	std::cout << "Test1: ";
	try
	{
		EventManager manager(globals);

		std::cout << "	PASS\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}

	std::cout << "Test2: ";
	try
	{
		int pipefd[2];

		EventManager manager(globals);
		Event event;

		pipe(pipefd);
		dup2(pipefd[0], STDIN_FILENO);

		event.setFd(STDIN_FILENO);
		FileDescriptor::setCloseOnExec_NonBlocking(event.getFd());
		event.setFlags(EPOLLIN);
		event.setHandler(EventHandler);


		manager.addEvent(event);

		const char* str = "	triggering event\n";
		write(pipefd[1], str, strlen(str));
		

		manager.waitEvents(-1);
		manager.distributeEvents();

		close(pipefd[1]);
		close(pipefd[0]);

		std::cout << "	PASS\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}

	return (0);
}

