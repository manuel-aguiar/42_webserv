

// C++ headers
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

// Project headers
#include "../StackArray.hpp"
#include "../../../Toolkit/_Tests/ToolkitDummy.hpp"
#include "../../../Toolkit/_Tests/ToolkitBase.hpp"
#include "../../../Toolkit/_Tests/ToolkitDerived.hpp"
#include "../../../Toolkit/TestHelpers/TestHelpers.h"

# include <vector>

int TestPart0(int testNumber)
{
	/*********************** ************************/
	TEST_INTRO(testNumber++);
	try
	{
		StackArray<int, 10>     array1(0);
        StackArray<int, 1>      array2(0);
        StackArray<int, 100>    array3(0);
		
		std::vector<StackArrayImpl<int>* > vec;

        vec.push_back(&array1);
        vec.push_back(&array2);
        vec.push_back(&array3);

        for (size_t i = 0; i < vec.size(); ++i)
        {
            size_t j = 0;
            for (; j < vec[i]->capacity(); ++j)
            {
            }
            EXPECT_EQUAL(j, vec[i]->capacity(), "size mismatch");
        }


		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

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

    return (testNumber);
}