// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <cstdlib>
#include <list>

// Project headers
# include "../StackCircularQueue.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../TestHelpers/TestHelpers.h"

int TestPart1(int testNumber)
{
    try
    {
        TEST_INTRO(testNumber++);

        const int queueSize = 10;
        const int frontNumber = 5;
        const int backNumber = 2;

        size_t expectedElemCount = 0;
        bool resultInsertion;
        bool expectedInsertion;

        StackCircularQueue<int, queueSize> queue;

        /************* */
        resultInsertion = queue.push_back(frontNumber);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");

        /************* */
        resultInsertion = queue.emplace_back(backNumber);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");

        /************* */
        EXPECT_EQUAL(queue[0], frontNumber, "index 0 doesn't match ");
        EXPECT_EQUAL(queue[1], backNumber, "index 1 doesn't match ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");
        EXPECT_EQUAL(queue.back(), backNumber, "back doesn't match ");
        EXPECT_EQUAL(queue.front(), frontNumber, "front doesn't match ");

        /************* */
        resultInsertion = queue.push_back(10);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");

        /************* */
        resultInsertion = queue.push_back(10);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");

        /************* */
        resultInsertion = queue.pop_back();
        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "pop result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");

        /************* */
        resultInsertion = queue.pop_back();
        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "pop result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");

        /************* */
        resultInsertion = queue.pop_back();
        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "pop result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");

        /************* */
        resultInsertion = queue.pop_back();
        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "pop result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");

        /************* */
        resultInsertion = queue.push_back(10);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    /******************************************************************** */

    try
    {
        TEST_INTRO(testNumber++);

        const int queueSize = 2;
        const int frontNumber = 5;
        const int backNumber = 2;

        size_t expectedElemCount = 0;
        bool resultInsertion;
        bool expectedInsertion;

        StackCircularQueue<int, queueSize> queue;

        /************* */
        resultInsertion = queue.push_front(backNumber);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");

        /************* */
        resultInsertion = queue.push_front(frontNumber);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");

        /************* */
        EXPECT_EQUAL(queue[0], frontNumber, "index 0 doesn't match ");
        EXPECT_EQUAL(queue[1], backNumber, "index 1 doesn't match ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");
        EXPECT_EQUAL(queue.back(), backNumber, "back doesn't match ");
        EXPECT_EQUAL(queue.front(), frontNumber, "front doesn't match ");

        /************* */
        resultInsertion = queue.push_front(10);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");

        /************* */
        resultInsertion = queue.push_front(10);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");

        /************* */
        resultInsertion = queue.pop_front();
        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "pop result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");

        /************* */
        resultInsertion = queue.pop_front();
        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "pop result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");

        /************* */
        resultInsertion = queue.pop_front();
        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "pop result is not the same ");

        /************* */
        resultInsertion = queue.pop_front();
        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "pop result is not the same ");

        /************* */
        resultInsertion = queue.push_back(10);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }




    TEST_INTRO(testNumber++);
    try
    {
        const int queueSize = 10;
        StackCircularQueue<int, queueSize> queue;

        queue.push_back(1);
        queue.push_back(2);

        StackCircularQueue<int, queueSize>::iterator it = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itEnd = queue.end();

        EXPECT_EQUAL(it != itEnd, true, "iterators, push_back, non-full queue, begin and end should not be equal");

        size_t i = 0;
        for ( ; it != itEnd; ++it, ++i)
        {
            EXPECT_EQUAL(*it, queue[i], "iterators, push_back, non-full queue, value mismatch");
        }

        EXPECT_EQUAL(i, queue.size(), "iterators, push_back, non-full queue, size mismatch");

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    // iterators, push_back, full queue
    TEST_INTRO(testNumber++);
    try
    {
        const int queueSize = 2;
        StackCircularQueue<int, queueSize> queue;

        queue.push_back(1);
        queue.push_back(2);

        StackCircularQueue<int, queueSize>::iterator it = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itEnd = queue.end();

        EXPECT_EQUAL(it != itEnd, true, "iterators, push_back, full queue, begin and end should not be equal");

        size_t i = 0;
        for ( ; it != itEnd; ++it, ++i)
        {
            EXPECT_EQUAL(*it, queue[i], "iterators, push_back, full queue, value mismatch");
        }

        EXPECT_EQUAL(i, queue.size(), "iterators, push_back, full queue, size mismatch");

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    // iterators, push_front, non-full queue
    TEST_INTRO(testNumber++);
    try
    {
        const int queueSize = 10;
        StackCircularQueue<int, queueSize> queue;

        queue.push_front(1);
        queue.push_front(2);

        StackCircularQueue<int, queueSize>::iterator it = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itEnd = queue.end();

        EXPECT_EQUAL(it != itEnd, true, "iterators, push_front, non-full queue, begin and end should not be equal");

        size_t i = 0;
        for ( ; it != itEnd; ++it, ++i)
        {
            EXPECT_EQUAL(*it, queue[i], "iterators, push_front, non-full queue, value mismatch");
        }

        EXPECT_EQUAL(i, queue.size(), "iterators, push_front, non-full queue, size mismatch");

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    // iterators, push_front, full queue
    TEST_INTRO(testNumber++);
    try
    {
        const int queueSize = 2;
        StackCircularQueue<int, queueSize> queue;

        queue.push_front(1);
        queue.push_front(2);

        StackCircularQueue<int, queueSize>::iterator it = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itEnd = queue.end();

        EXPECT_EQUAL(it != itEnd, true, "iterators, push_front, full queue, begin and end should not be equal");

        size_t i = 0;
        for ( ; it != itEnd; ++it, ++i)
        {
            EXPECT_EQUAL(*it, queue[i], "iterators, push_front, full queue, value mismatch");
        }

        EXPECT_EQUAL(i, queue.size(), "iterators, push_front, full queue, size mismatch");

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    // copy constructor, full copy-from
    TEST_INTRO(testNumber++);
    try
    {
        const int queueSize = 2;
        StackCircularQueue<int, queueSize> queue;

        queue.push_back(1);
        queue.push_back(2);

        StackCircularQueue<int, queueSize> copy(queue);

        EXPECT_EQUAL(queue.size(), copy.size(), "copy constructor, full copy-from, size mismatch");

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = copy.begin();

        for ( ; itOriginal != queue.end() && itCopy != copy.end(); ++itOriginal, ++itCopy)
        {
            EXPECT_EQUAL(*itOriginal, *itCopy, "copy constructor, full copy-from, value mismatch");
        }

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    // copy constructor, not-full copy-from
    TEST_INTRO(testNumber++);
    try
    {
        const int queueSize = 10;
        StackCircularQueue<int, queueSize> queue;

        queue.push_back(1);
        queue.push_back(2);

        StackCircularQueue<int, queueSize> copy(queue);

        EXPECT_EQUAL(queue.size(), copy.size(), "copy constructor, not-full copy-from, size mismatch");

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = copy.begin();

        for ( ; itOriginal != queue.end() && itCopy != copy.end(); ++itOriginal, ++itCopy)
        {
            EXPECT_EQUAL(*itOriginal, *itCopy, "copy constructor, not-full copy-from value mismatch");
        }

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    // copy constructor, empty copy-from
    TEST_INTRO(testNumber++);
    try
    {
        const int queueSize = 10;
        StackCircularQueue<int, queueSize> queue;

        StackCircularQueue<int, queueSize> copy(queue);

        EXPECT_EQUAL(queue.size(), copy.size(), "copy constructor, empty copy-from, size mismatch");

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = copy.begin();

        for ( ; itOriginal != queue.end() && itCopy != copy.end(); ++itOriginal, ++itCopy)
        {
            EXPECT_EQUAL(*itOriginal, *itCopy, "copy constructor, empty copy-from value mismatch");
        }

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    // copy assignment, full copy-from, empty copy-to
    TEST_INTRO(testNumber++);
    try
    {
        const int queueSize = 2;
        StackCircularQueue<int, queueSize> queue;

        queue.push_back(1);
        queue.push_back(2);

        StackCircularQueue<int, queueSize> assign(3);   //initialize elements to '3'

        assign = queue;

        EXPECT_EQUAL(queue.size(), assign.size(), "assignment, full copy-from, empty copy-to, size mismatch");

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            EXPECT_EQUAL(*itOriginal, *itCopy, "assignment, full copy-from, empty copy-to, value mismatch");
        }

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    // copy assignment, non-full copy-from, empty copy-to
    TEST_INTRO(testNumber++);
    try
    {
        const int queueSize = 10;
        StackCircularQueue<int, queueSize> queue(5); //initialize elements to 5

        queue.push_back(1);
        queue.push_back(2);

        StackCircularQueue<int, queueSize> assign(7); //initialize elements to 7

        assign = queue;

        EXPECT_EQUAL(queue.size(), assign.size(), "assignment, non-full copy-from, empty copy-to, size mismatch");

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            EXPECT_EQUAL(*itOriginal, *itCopy, "assignment, non-full copy-from, empty copy-to, value mismatch");
        }

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
////////////////////////////////////////////////////////////////////////////////////////////
    //copy assignment, empty copy-from, empty copy-to
    TEST_INTRO(testNumber++);
    try
    {
        const int queueSize = 10;
        StackCircularQueue<int, queueSize> queue;
        StackCircularQueue<int, queueSize> assign;

        assign = queue;

        EXPECT_EQUAL(queue.size(), assign.size(), "assignment, empty copy-from, empty copy-to, size mismatch");

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            EXPECT_EQUAL(*itOriginal, *itCopy, "assignment, empty copy-from, empty copy-to, value mismatch");
        }

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    //copy assignment, full copy-from, non-empty copy-to
    TEST_INTRO(testNumber++);
    try
    {
        const int queueSize = 2;
        StackCircularQueue<int, queueSize> queue;

        queue.push_back(1);
        queue.push_back(2);

        StackCircularQueue<int, queueSize> assign;
        assign.push_back(123);

        assign = queue;

        EXPECT_EQUAL(queue.size(), assign.size(), "assignment, full copy-from, non-empty copy-to, size mismatch");

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            EXPECT_EQUAL(*itOriginal, *itCopy, "assignment, full copy-from, non-empty copy-to, value mismatch");
        }

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    //copy assignment, non-full copy-from, non-empty copy-to
    TEST_INTRO(testNumber++);
    try
    {
        const int queueSize = 10;
        StackCircularQueue<int, queueSize> queue;

        queue.push_back(1);
        queue.push_back(2);

        StackCircularQueue<int, queueSize> assign;
        assign.push_back(123);

        assign = queue;

        EXPECT_EQUAL(queue.size(), assign.size(), "assignment, non-full copy-from, non-empty copy-to, size mismatch");

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            EXPECT_EQUAL(*itOriginal, *itCopy, "assignment, non-full copy-from, non-empty copy-to, value mismatch");
        }

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    //copy assignment, empty copy-from, non-empty copy-to
    TEST_INTRO(testNumber++);
    try
    {
        const int queueSize = 10;
        StackCircularQueue<int, queueSize> queue;

        StackCircularQueue<int, queueSize> assign;
        assign.push_back(123);

        assign = queue;

        EXPECT_EQUAL(queue.size(), assign.size(), "assignment, empty copy-from, non-empty copy-to, size mismatch");

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            EXPECT_EQUAL(*itOriginal, *itCopy, "assignment, empty copy-from, non-empty copy-to, value mismatch");
        }

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    //copy assignment, full copy-from, full copy-to
    TEST_INTRO(testNumber++);
    try
    {
        const int queueSize = 2;
        StackCircularQueue<int, queueSize> queue;

        queue.push_back(1);
        queue.push_back(2);

        StackCircularQueue<int, queueSize> assign;

        assign.push_back(3);
        assign.push_back(4);

        assign = queue;

        EXPECT_EQUAL(queue.size(), assign.size(), "assignment, full copy-from, full copy-to, size mismatch");

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            EXPECT_EQUAL(*itOriginal, *itCopy, "assignment, full copy-from, full copy-to, value mismatch");
        }

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    //copy assignment, non-full copy-from, full copy-to
    TEST_INTRO(testNumber++);
    try
    {
        const int queueSize = 2;
        StackCircularQueue<int, queueSize> queue;

        queue.push_back(1);

        StackCircularQueue<int, queueSize> assign;

        assign.push_back(3);
        assign.push_back(4);

        assign = queue;

        EXPECT_EQUAL(queue.size(), assign.size(), "assignment, non-full copy-from, full copy-to, size mismatch");

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            EXPECT_EQUAL(*itOriginal, *itCopy, "assignment, non-full copy-from, full copy-to, value mismatch");
        }

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    //copy assignment, empty copy-from, full copy-to
    TEST_INTRO(testNumber++);
    try
    {
        const int queueSize = 2;
        StackCircularQueue<int, queueSize> queue;

        StackCircularQueue<int, queueSize> assign;

        assign.push_back(3);
        assign.push_back(4);

        assign = queue;

        EXPECT_EQUAL(queue.size(), assign.size(), "assignment, empty copy-from, full copy-to, size mismatch");

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            EXPECT_EQUAL(*itOriginal, *itCopy, "assignment, empty copy-from, full copy-to, value mismatch");
        }

        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    return (testNumber);
}

