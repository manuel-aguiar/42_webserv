

// Test subject
# include "../EventCallback.hpp"

//Project headers
# include "../../../Toolkit/_Tests/TestHelpers.h"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include <iostream>

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


int main(void)
{
	int testNumber = 1;

	std::cout << "\n*************** Callback tests ***************" << std::endl;

	try
	{
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
		
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	std::cout << "*********************************************\n" << std::endl;
}