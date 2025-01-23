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
#include "../../../_Tests/TestHelpers.h"

int TestPart2(int testNumber)
{
    /********************************************* */
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

        const int arraySize = 100;

        std::vector<int> std;
        StackArray<int, arraySize> array;

        for (int i = 0; i < arraySize; ++i)
        {
            std.push_back(i);
            array.emplace_back(i);
        }

        // Compare sizes
        TestHelpers::assertEqual(std.size(), array.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

        // Compare contents via iterators
        StackArray<int, 100>::iterator it = array.begin();
        std::vector<int>::iterator iter = std.begin();

        for (; it != array.end() && iter != std.end(); ++it, ++iter)
        {
            TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
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
            TestHelpers::assertEqual(*it, *iter, "copy assignment value mismatch", __FILE__, __LINE__, __FUNCTION__);
        }

        TestHelpers::assertEqual(std.size(), assign.size(), "copy assignment size mismatch", __FILE__, __LINE__, __FUNCTION__);

        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "	FAILED: " << e.what() << std::endl;
    }

    /********************************************* */
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

        const int arraySize = 100;

        std::vector<int> std;
        StackArray<int, arraySize> array;

        for (size_t i = 0; i < array.capacity(); ++i)
        {
            std.push_back(i);
            array.emplace_back(i);
        }

        TestHelpers::assertEqual(std.size(), array.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

        // Compare contents via iterators
        StackArray<int, 100>::iterator it = array.begin();
        std::vector<int>::iterator iter = std.begin();

        for (; it != array.end() && iter != std.end(); ++it, ++iter)
        {
            TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
        }

        // Test copy constructor
        StackArray<int, 100> copy(array);

        it = copy.begin();
        iter = std.begin();

        for (; it != copy.end() && iter != std.end(); ++it, ++iter)
        {
            TestHelpers::assertEqual(*it, *iter, "copy constructor value mismatch", __FILE__, __LINE__, __FUNCTION__);
        }

        TestHelpers::assertEqual(std.size(), copy.size(), "copy constructor size mismatch", __FILE__, __LINE__, __FUNCTION__);

        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "	FAILED: " << e.what() << std::endl;
    }

    /********************************************* */

    try
    {
        const int arraySize = 100;

        std::cout << "TEST " << testNumber++ << ": ";
        std::vector<int> std;
        StackArray<int, arraySize> array(5); // All elements initialized to 5

        for (size_t i = 0; i < array.capacity(); ++i)
        {
            std.push_back(5);
        }

        TestHelpers::assertEqual(std.size(), array.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

        // Compare contents via iterators
        StackArray<int, arraySize>::iterator it = array.begin();
        std::vector<int>::iterator iter = std.begin();

        for (; it != array.end() && iter != std.end(); ++it, ++iter)
        {
            TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
        }

        // Test copy constructor
        StackArray<int, arraySize> copy(array);

        it = copy.begin();
        iter = std.begin();

        for (; it != copy.end() && iter != std.end(); ++it, ++iter)
        {
            TestHelpers::assertEqual(*it, *iter, "copy constructor value mismatch", __FILE__, __LINE__, __FUNCTION__);
        }

        TestHelpers::assertEqual(std.size(), copy.size(), "copy constructor size mismatch", __FILE__, __LINE__, __FUNCTION__);

        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "	FAILED: " << e.what() << std::endl;
    }

    return testNumber;
}
