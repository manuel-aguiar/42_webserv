

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
# include "../../../TestHelpers/TestHelpers.h"


int TestPart1(int testNumber)
{
	/*********************** ************************/
	TEST_INTRO(testNumber++);
	try
	{
		const size_t arraySize = 100;
		StackArray<int, arraySize> array;
		
		EXPECT_EQUAL(array.capacity(), arraySize, "size mismatch");


		for (int i = 0; i < 100; ++i)
		{
			array.emplace_back(i);
		}

		EXPECT_EQUAL(array.size(), arraySize, "size mismatch");

		for (int i = 0; i < 100; ++i)
			EXPECT_EQUAL(array[i], i, "value mismatch");


		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/*********************** ************************/
	TEST_INTRO(testNumber++);
	try
	{
		const size_t arraySize = 100;
		const int copyInit = 123;

		// copy constructs the elements based on the one element passed
		StackArray<int, arraySize> array(copyInit);

		EXPECT_EQUAL(array.capacity(), arraySize, "size mismatch");

		for (size_t i = 0; i < array.size(); ++i)
			EXPECT_EQUAL(array[i], copyInit, "value mismatch");

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}


	/*************************************************/

	try
	{
		TEST_INTRO(testNumber++);


		const size_t arraySize = 100;

		StackArray<ToolkitDummy, arraySize> array;
		for (size_t i = 0; i < array.capacity(); ++i)
		{
			array.emplace_back(i);
		}

		EXPECT_EQUAL(array.size(), arraySize, "size mismatch");
			
		for (size_t i = 0; i < array.size(); ++i)
			EXPECT_EQUAL(array[i] == i, true, "value mismatch");
		
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/******************* ***** ************************/

	try
	{
		TEST_INTRO(testNumber++);
		StackArray<int, 100> array;

		for (int i = 0; i < 100; ++i)
		{
			array.emplace_back(i);
		}
		array.pop_back();

		EXPECT_EQUAL(array.size(), 99, "size mismatch");
		EXPECT_EQUAL(array.back(), 98, "value mismatch");

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

/******************* *** ************************/
	try
	{
		TEST_INTRO(testNumber++);
		StackArray<int, 100> array;
		for (int i = 0; i < 100; ++i)
		{
			array.emplace_back(i);
		}

		// Check front and back
		EXPECT_EQUAL(array.front(), 0, "value mismatch");
		EXPECT_EQUAL(array.back(), 99, "value mismatch");

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/******************* *** ************************/

	try
	{
		TEST_INTRO(testNumber++);
		StackArray<int, 5> array;
		array.emplace_back(1);
		array.emplace_back(2);
		array.emplace_back(3);

		EXPECT_EQUAL(array.size(), 3, "size mismatch");

		array.emplace_back(4);
		array.emplace_back(5);

		EXPECT_EQUAL(array.size(), 5, "size mismatch");

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/******************* *** ************************/

	try
	{
		TEST_INTRO(testNumber++);

		StackArray<int, 5> array;
		array.emplace_back(1);
		array.emplace_back(2);
		array.emplace_back(3);

		// Test iterator
		StackArray<int, 5>::iterator it = array.begin();
		EXPECT_EQUAL(*it, 1, "value mismatch");

		it++;
		EXPECT_EQUAL(*it, 2, "value mismatch");

		it++;
		EXPECT_EQUAL(*it, 3, "value mismatch");

		it++;
		EXPECT_EQUAL(it == array.end(), true, "iterator should be at end");

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/******************* *** ************************/
	/* just strings... */
	try
	{
		TEST_INTRO(testNumber++);
		StackArray<std::string, 3> array;
		array.emplace_back("Hello");
		array.emplace_back("World");
		array.emplace_back("StackArray");

		EXPECT_EQUAL(array[0], std::string("Hello"), "value mismatch");
		EXPECT_EQUAL(array[1], std::string("World"), "value mismatch");
		EXPECT_EQUAL(array[2], std::string("StackArray"), "value mismatch");


		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

    return (testNumber);
}
