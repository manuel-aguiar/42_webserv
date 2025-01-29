

// Test subject
# include "../EventCallback.hpp"

//Project headers
# include "../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include <iostream>
# include <cstdio>
# include <unistd.h>

class User
{
	public:
		void doSomething()
		{
			m_data = 42;
		}

		static void MyCallback_doSomething(EventCallback& event)
		{
			User* me = reinterpret_cast<User*>(event.accessUser());
			me->doSomething();
		}

		int getData() const
		{
			return (m_data);
		}

	private:
		int	m_data;
};

void	PrintHelloWorld(EventCallback& callback)
{
	(void)callback;

	std::cout << "Hello World!";
	std::cout.flush();
}



int main(void)
{
	int testNumber = 1;

	std::cout << "\n*************** EventCallback tests ***************" << std::endl;

	try
	{

		std::cout << "TEST " << testNumber++ << ": ";

		User			user;
		EventCallback 	callback;

		callback.setUser(&user);
		callback.setHandler(&User::MyCallback_doSomething);

		callback.handle();

		assert(user.getData() == 42);

		if (user.getData() != 42)
			throw std::runtime_error("EventCallback failed to handle, got " 
			+ StringUtils::to_string(user.getData()) + ", expected " 
			+ StringUtils::to_string(42) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		
		std::cout << "	PASSED (passing a static member function that typecasts the User and performs a user action)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	try
	{

		std::cout << "TEST " << testNumber++ << ": ";
		std::cout.flush();

		//some pipes to capture stdout
		int pipefd[2];
		int duplicate = dup(STDOUT_FILENO);
		pipe(pipefd);
		dup2(pipefd[1], STDOUT_FILENO);

		////// Actual Test  ///////
		EventCallback 	callback;
		char			buffer[64];

		callback.setHandler(&PrintHelloWorld);
		callback.handle();
		////////////////////////////

		int bytesRead = ::read(pipefd[0], buffer, 64);
		buffer[bytesRead] = '\0';

		// restore stdout
		dup2(duplicate, STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		close(duplicate);

		if (std::string(buffer) != "Hello World!")
			throw std::runtime_error("EventCallback failed to handle, got " 
			+ std::string(buffer) + ", expected " 
			+ std::string("Hello World!") + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		
		std::cout << "	PASSED (passing a non-member function pointer and no user)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	//valgrind, yes
	close(STDOUT_FILENO);

	std::cout << "*********************************************\n" << std::endl;
}