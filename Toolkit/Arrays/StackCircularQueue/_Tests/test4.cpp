// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <list>

// Project headers
# include "../StackCircularQueue.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../TestHelpers/TestHelpers.h"


int TestPart4(int testNumber)
{
    try
    {
        TEST_INTRO(testNumber++);

        const int queueSize = 200;
        std::list<int> list;
        StackCircularQueue<int, queueSize> queue;

        for (int i = 0; i < 100; ++i)
        {
            list.push_back(i);
            queue.push_back(i);

            list.push_front(i);
            queue.push_front(i);
        }

        EXPECT_EQUAL(list.size(), queue.size(), "size mismatch in first test");

        StackCircularQueue<int, queueSize>::iterator it = queue.begin();
        std::list<int>::iterator iter = list.begin();
        for (; it != queue.end() && iter != list.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "value mismatch in first test");
        }

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

        const int queueSize = 100;
        std::list<ToolkitDummy> std;
        StackCircularQueue<ToolkitDummy, queueSize> queue;

        for (int i = 0; i < 100; ++i)
        {
            std.push_back(i);
            queue.push_back(i);
        }

        EXPECT_EQUAL(std.size(), queue.size(), "size mismatch in second test");

        StackCircularQueue<ToolkitDummy, queueSize>::iterator it = queue.begin();
        std::list<ToolkitDummy>::iterator iter = std.begin();
        for (; it != queue.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "value mismatch in second test");
        }

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    /******************* *****************************/

    try
    {
        TEST_INTRO(testNumber++);

        const int queueSize = 100;
        std::list<ToolkitDummy> std;
        StackCircularQueue<ToolkitDummy, queueSize> queue;

        for (int i = 0; i < 100; ++i)
        {
            std.push_back(i);
            queue.emplace_back(i);
        }

        EXPECT_EQUAL(std.size(), queue.size(), "size mismatch in third test");

        StackCircularQueue<ToolkitDummy, queueSize>::iterator it = queue.begin();
        std::list<ToolkitDummy>::iterator iter = std.begin();
        for (; it != queue.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "value mismatch in third test");
        }

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

        const int queueSize = 200;
        std::list<ToolkitDummy> list;
        StackCircularQueue<ToolkitDummy, queueSize> queue;

        for (int i = 0; i < 100; ++i)
        {
            list.push_back(i);
            queue.emplace_back(i);
        }

        EXPECT_EQUAL(list.size(), queue.size(), "size mismatch in fourth test");

        StackCircularQueue<ToolkitDummy, queueSize>::iterator it = queue.begin();
        std::list<ToolkitDummy>::iterator iter = list.begin();

        for (; it != queue.end() && iter != list.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "value mismatch in fourth test");
        }

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    /***************************************** */

    return (testNumber);
}
