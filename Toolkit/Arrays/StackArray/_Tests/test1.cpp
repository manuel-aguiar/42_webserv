

// C++ headers
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

// Project headers
#include "../StackArray.hpp"
#include "../../../_Tests/ToolkitDummy.hpp"
#include "../../../_Tests/ToolkitBase.hpp"
#include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../_Tests/TestHelpers.h"


int TestPart1(int testNumber)
{
	/*********************** ************************/
	std::cout << "TEST " << testNumber++ << ": ";
	try
	{
		const size_t arraySize = 100;
		StackArray<int, arraySize> array;
		
		TestHelpers::assertEqual(array.capacity(), arraySize, "size mismatch", __FILE__, __LINE__, __FUNCTION__);


		for (int i = 0; i < 100; ++i)
		{
			array.emplace_back(i);
		}

		TestHelpers::assertEqual(array.size(), arraySize, "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		for (int i = 0; i < 100; ++i)
			TestHelpers::assertEqual(array[i], i, "value mismatch", __FILE__, __LINE__, __FUNCTION__);


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	/*********************** ************************/
	std::cout << "TEST " << testNumber++ << ": ";
	try
	{
		const size_t arraySize = 100;
		const int copyInit = 123;

		// copy constructs the elements based on the one element passed
		StackArray<int, arraySize> array(copyInit);

		TestHelpers::assertEqual(array.capacity(), arraySize, "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		for (size_t i = 0; i < array.size(); ++i)
			TestHelpers::assertEqual(array[i], copyInit, "value mismatch", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}


	/*************************************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";


		const size_t arraySize = 100;

		StackArray<ToolkitDummy, arraySize> array;
		for (size_t i = 0; i < array.capacity(); ++i)
		{
			array.emplace_back(i);
		}

		TestHelpers::assertEqual(array.size(), arraySize, "size mismatch", __FILE__, __LINE__, __FUNCTION__);
			
		for (size_t i = 0; i < array.size(); ++i)
			TestHelpers::assertEqual(array[i] == i, true, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	/******************* ***** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		StackArray<int, 100> array;

		for (int i = 0; i < 100; ++i)
		{
			array.emplace_back(i);
		}
		array.pop_back();

		TestHelpers::assertEqual(array.size(), (size_t)99, "size mismatch", __FILE__, __LINE__, __FUNCTION__);
		TestHelpers::assertEqual(array.back(), 98, "value mismatch", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

/******************* *** ************************/
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		StackArray<int, 100> array;
		for (int i = 0; i < 100; ++i)
		{
			array.emplace_back(i);
		}

		// Check front and back
		TestHelpers::assertEqual(array.front(), 0, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		TestHelpers::assertEqual(array.back(), 99, "value mismatch", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		StackArray<int, 5> array;
		array.emplace_back(1);
		array.emplace_back(2);
		array.emplace_back(3);

		TestHelpers::assertEqual(array.size(), (size_t)3, "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		array.emplace_back(4);
		array.emplace_back(5);

		TestHelpers::assertEqual(array.size(), (size_t)5, "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		StackArray<int, 5> array;
		array.emplace_back(1);
		array.emplace_back(2);
		array.emplace_back(3);

		// Test iterator
		StackArray<int, 5>::iterator it = array.begin();
		TestHelpers::assertEqual(*it, 1, "value mismatch", __FILE__, __LINE__, __FUNCTION__);

		it++;
		TestHelpers::assertEqual(*it, 2, "value mismatch", __FILE__, __LINE__, __FUNCTION__);

		it++;
		TestHelpers::assertEqual(*it, 3, "value mismatch", __FILE__, __LINE__, __FUNCTION__);

		it++;
		TestHelpers::assertEqual(it == array.end(), true, "iterator should be at end", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	/******************* *** ************************/
	/* just strings... */
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		StackArray<std::string, 3> array;
		array.emplace_back("Hello");
		array.emplace_back("World");
		array.emplace_back("StackArray");

		TestHelpers::assertEqual(array[0], std::string("Hello"), "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		TestHelpers::assertEqual(array[1], std::string("World"), "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		TestHelpers::assertEqual(array[2], std::string("StackArray"), "value mismatch", __FILE__, __LINE__, __FUNCTION__);


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    return (testNumber);
}
