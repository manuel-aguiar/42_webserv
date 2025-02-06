

// C++ headers
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

// Project headers
#include "../../StackArray/StackArray.hpp"
#include "../../HeapArray/HeapArray.hpp"
#include "../../../_Tests/ToolkitDummy.hpp"
#include "../../../_Tests/ToolkitBase.hpp"
#include "../../../_Tests/ToolkitDerived.hpp"
#include "../../../TestHelpers/TestHelpers.h"

# include <vector>

int TestPart1(int testNumber)
{
	/*********************** ************************/
	TEST_INTRO(testNumber++);
	try
	{
		StackArray<int, 10>     array1;
        StackArray<int, 1>      array2;
        StackArray<int, 100>    array3;
		HeapArray<int>          array4(100);
		HeapArray<int>          array5(10);
		HeapArray<int>          array6(array5);

		std::vector<ArrayFixedSizeImpl<int>* > vec;

        vec.push_back(&array1);
        vec.push_back(&array2);
        vec.push_back(&array3);
		vec.push_back(&array4);
		vec.push_back(&array5);
		vec.push_back(&array6);

        for (size_t i = 0; i < vec.size(); ++i)
        {
            size_t j = 0;
            for (; j < vec[i]->capacity(); ++j)
            {
				vec[i]->push_back(j);
            }
            EXPECT_EQUAL(j, vec[i]->capacity(), "size mismatch");
        }

        for (size_t i = 0; i < vec.size(); ++i)
        {
            size_t j = 0;
            for (; j < vec[i]->size(); ++j)
            {
				EXPECT_EQUAL((*vec[i])[j], j, "value mismatch");
				(*vec[i])[j] = 100;
				EXPECT_EQUAL((*vec[i])[j], 100, "value mismatch");
			}
			EXPECT_EQUAL(j, vec[i]->capacity(), "size mismatch");
		}

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

    return (testNumber);
}