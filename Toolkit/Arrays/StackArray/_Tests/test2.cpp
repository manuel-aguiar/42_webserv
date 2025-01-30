// C++ headers
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <vector>

// Project headers
#include "../StackArray.hpp"
#include "../../../_Tests/ToolkitDummy.hpp"
#include "../../../_Tests/ToolkitBase.hpp"
#include "../../../_Tests/ToolkitDerived.hpp"
#include "../../../TestHelpers/TestHelpers.h"

int TestPart2(int testNumber)
{
    /********************************************* */
    try
    {
        TEST_INTRO(testNumber++);

        const int arraySize = 100;

        std::vector<int> std;
        StackArray<int, arraySize> array;

        for (int i = 0; i < arraySize; ++i)
        {
            std.push_back(i);
            array.emplace_back(i);
        }

        // Compare sizes
        EXPECT_EQUAL(std.size(), array.size(), "size mismatch");

        // Compare contents via iterators
        StackArray<int, 100>::iterator it = array.begin();
        std::vector<int>::iterator iter = std.begin();

        for (; it != array.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "value mismatch");
        }

        // Test copy assignment
        StackArray<int, 100> assign;

        assign.push_back(-123);
        assign.push_back(-456);

        assign = array;

        it = assign.begin();
        iter = std.begin();

        for (; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "copy assignment value mismatch");
        }

        EXPECT_EQUAL(std.size(), assign.size(), "copy assignment size mismatch");

        TEST_PASSED;
    }
    catch (const std::exception &e)
    {
        std::cout << "	FAILED: " << e.what() << std::endl;
    }

    /********************************************* */
    try
    {
        TEST_INTRO(testNumber++);

        const int arraySize = 100;

        std::vector<int> std;
        StackArray<int, arraySize> array;

        for (size_t i = 0; i < array.capacity(); ++i)
        {
            std.push_back(i);
            array.emplace_back(i);
        }

        EXPECT_EQUAL(std.size(), array.size(), "size mismatch");

        // Compare contents via iterators
        StackArray<int, 100>::iterator it = array.begin();
        std::vector<int>::iterator iter = std.begin();

        for (; it != array.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "value mismatch");
        }

        // Test copy constructor
        StackArray<int, 100> copy(array);

        it = copy.begin();
        iter = std.begin();

        for (; it != copy.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "copy constructor value mismatch");
        }

        EXPECT_EQUAL(std.size(), copy.size(), "copy constructor size mismatch");

        TEST_PASSED;
    }
    catch (const std::exception &e)
    {
        std::cout << "	FAILED: " << e.what() << std::endl;
    }

    /********************************************* */

    try
    {
        const int arraySize = 100;

        TEST_INTRO(testNumber++);
        std::vector<int> std;
        StackArray<int, arraySize> array(5); // All elements initialized to 5

        for (size_t i = 0; i < array.capacity(); ++i)
        {
            std.push_back(5);
        }

        EXPECT_EQUAL(std.size(), array.size(), "size mismatch");

        // Compare contents via iterators
        StackArray<int, arraySize>::iterator it = array.begin();
        std::vector<int>::iterator iter = std.begin();

        for (; it != array.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "value mismatch");
        }

        // Test copy constructor
        StackArray<int, arraySize> copy(array);

        it = copy.begin();
        iter = std.begin();

        for (; it != copy.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "copy constructor value mismatch");
        }

        EXPECT_EQUAL(std.size(), copy.size(), "copy constructor size mismatch");

        TEST_PASSED;
    }
    catch (const std::exception &e)
    {
        std::cout << "	FAILED: " << e.what() << std::endl;
    }

    return testNumber;
}
