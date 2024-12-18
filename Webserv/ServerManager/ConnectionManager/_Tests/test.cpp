

# include "../ConnectionManager.hpp"
# include "TestDependencies.hpp"
# include <iostream>
# include "../../../Globals/Globals.hpp"

int main(void)
{
	/***************************************** */
	Globals			globals(NULL, NULL, NULL, NULL);

	Nginx_MemoryPool* pool = Nginx_MemoryPool::create(4096, 1);

	std::cout << "Test1: ";
	try
	{
		ConnectionManager manager(1, *pool, globals);

		std::cout << "	PASS\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}

	std::cout << "Test2: ";
	try
	{
		ConnectionManager manager(1, *pool, globals);
		Connection* conn1 = manager.provideConnection();

		if (conn1 == NULL)
			throw std::runtime_error("provideConnection() returned NULL, but there is one connection");


		Connection* conn2 = manager.provideConnection();

		if (conn2 != NULL)
			throw std::runtime_error("provideConnection() did not return NULL, but there are no connections");

		manager.returnConnection(*conn1);

		
		conn2 = manager.provideConnection();

		if (conn2 != conn1)
			throw std::runtime_error("provideConnection() should have returned the same we gave back");

		std::cout << "	PASS\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}

	std::cout << "Test3: ";
	try
	{
		ConnectionManager manager(5, *pool, globals);
		Connection* conn1;
		conn1 = manager.provideConnection();
		conn1 = manager.provideConnection();
		conn1 = manager.provideConnection();
		conn1 = manager.provideConnection();
		conn1 = manager.provideConnection();


		if (conn1 == NULL)
			throw std::runtime_error("provideConnection() returned NULL, but there is one connection");

		Connection* conn2 = manager.provideConnection();

		if (conn2 != NULL)
			throw std::runtime_error("provideConnection() did not return NULL, but there are no connections");

		manager.returnConnection(*conn1);
		conn2 = manager.provideConnection();

		if (conn2 != conn1)
			throw std::runtime_error("provideConnection() should have returned the same we gave back");

		std::cout << "	PASS\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}

	/************************************************************/
	/************************************************************/
	/************************************************************/
	/************************************************************/


	std::cout << "Test4: ";
	try
	{
		ConnectionManager manager(1, *pool, globals);

		Connection* conn1 = manager.provideConnection();
		manager.returnConnection(*conn1);
		//manager.returnConnection(*conn1);				//correctly raises abort in DEBUG mode
		/*
			ConnectionManager manager2(1, *pool, globals);
			Connection* foreign = manager2.provideConnection();
			manager.returnConnection(*foreign);			//correctly raises abort in DEBUG mode	
		*/
		(void)conn1;

		std::cout << "	PASS\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}

	pool->destroy();

	return (0);
}

