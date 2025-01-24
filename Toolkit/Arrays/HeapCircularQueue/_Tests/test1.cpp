
// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <cstdlib>
#include <list>

// Project headers
# include "../HeapCircularQueue.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"

# include "../../../TestHelpers/TestHelpers.h"

int TestPart1(int testNumber)
{

    try
	{
		std::cout << "TEST " << testNumber++ << ": ";

        const int queueSize = 10;
        const int frontNumber = 5;
        const int backNumber = 2;

        size_t expectedElemCount = 0;
        bool resultInsertion;
        bool expectedInsertion;

		HeapCircularQueue<int> 			queue(queueSize);

        /************* */
        
        resultInsertion = queue.push_back(frontNumber);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;

        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "insertion result is not the same ", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "size is not the same ", __FILE__, __LINE__, __FUNCTION__);

        /************* */
               
        resultInsertion = queue.emplace_back(backNumber);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;

        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "insertion result is not the same ", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "size is not the same ", __FILE__, __LINE__, __FUNCTION__);

        /************* */

        TestHelpers::assertEqual(queue[0], frontNumber, "index 0 doesn't match ", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue[1], backNumber, "index 1 doesn't match ", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "size is not the same ", __FILE__, __LINE__, __FUNCTION__);        
        TestHelpers::assertEqual(queue.back(), backNumber, "back doesn't match ", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.front(), frontNumber, "front doesn't match ", __FILE__, __LINE__, __FUNCTION__);        

        resultInsertion = queue.push_back(10);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;

        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "insertion result is not the same ", __FILE__, __LINE__, __FUNCTION__);

        /************* */
       
        resultInsertion = queue.push_back(10);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "insertion result is not the same ", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "size is not the same ", __FILE__, __LINE__, __FUNCTION__);

        /************* */
        
        resultInsertion = queue.pop_back();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "insertion result is not the same ", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "size is not the same ", __FILE__, __LINE__, __FUNCTION__);

        /************* */

        resultInsertion = queue.pop_back();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "insertion result is not the same ", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "size is not the same ", __FILE__, __LINE__, __FUNCTION__);

        /************* */

        resultInsertion = queue.pop_back();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "insertion result is not the same ", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "size is not the same ", __FILE__, __LINE__, __FUNCTION__);

    /************* */

        resultInsertion = queue.pop_back();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "insertion result is not the same ", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "size is not the same ", __FILE__, __LINE__, __FUNCTION__);

        /************* */

        resultInsertion = queue.push_back(10);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += resultInsertion;
        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "insertion result is not the same ", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "size is not the same ", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}




/******************************************************************** */
/******************************************************************** */

    try
	{
		std::cout << "TEST " << testNumber++ << ": ";

        const int queueSize = 2;
        const int frontNumber = 5;
        const int backNumber = 2;

        size_t expectedElemCount = 0;
        bool resultInsertion;
        bool expectedInsertion;

		HeapCircularQueue<int> 			queue(queueSize);

        /************* */
        
        resultInsertion = queue.push_front(backNumber);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "push_front result mismatch", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "push_front size mismatch", __FILE__, __LINE__, __FUNCTION__);

        /************* */
               
        resultInsertion = queue.push_front(frontNumber);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "push_front result mismatch", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "push_front size mismatch", __FILE__, __LINE__, __FUNCTION__);

        /************* */

        TestHelpers::assertEqual(queue[0], frontNumber, "index 0 mismatch", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue[1], backNumber, "index 1 mismatch", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "size mismatch", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.back(), backNumber, "back mismatch", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.front(), frontNumber, "front mismatch", __FILE__, __LINE__, __FUNCTION__);

        resultInsertion = queue.push_front(10);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "push_front result mismatch", __FILE__, __LINE__, __FUNCTION__);

        /************* */
       
        resultInsertion = queue.push_front(10);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "push_front result mismatch", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "size mismatch after push_front", __FILE__, __LINE__, __FUNCTION__);

        /************* */
        
        resultInsertion = queue.pop_front();
        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "pop_front result mismatch", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "pop_front size mismatch", __FILE__, __LINE__, __FUNCTION__);

        /************* */

        resultInsertion = queue.pop_front();
        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "pop_front result mismatch", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "pop_front size mismatch", __FILE__, __LINE__, __FUNCTION__);

        /************* */

        resultInsertion = queue.pop_front();
        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "pop_front result mismatch", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "pop_front size mismatch", __FILE__, __LINE__, __FUNCTION__);

    /************* */

        resultInsertion = queue.pop_front();
        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "pop_front result mismatch", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "pop_front size mismatch", __FILE__, __LINE__, __FUNCTION__);

        /************* */

        resultInsertion = queue.push_front(10);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += resultInsertion;
        TestHelpers::assertEqual(resultInsertion, expectedInsertion, "push_front result mismatch", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(queue.size(), expectedElemCount, "push_front size mismatch", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    // iterators, empty queue
    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        HeapCircularQueue<int> queue(10);

        HeapCircularQueue<int>::iterator it = queue.begin();
        HeapCircularQueue<int>::iterator itEnd = queue.end();

        TestHelpers::assertEqual(it == itEnd, true, "iterators, empty queue, begin and end should be equal", __FILE__, __LINE__, __FUNCTION__);

        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "	FAILED: " << e.what()  << std::endl;
    }

    // iterators, push_back, non-full queue
    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        HeapCircularQueue<int> queue(10);

        queue.push_back(1);
        queue.push_back(2);

        HeapCircularQueue<int>::iterator it = queue.begin();
        HeapCircularQueue<int>::iterator itEnd = queue.end();

        TestHelpers::assertEqual(it != itEnd, true, "iterators, push_back, non-full queue, begin and end should not be equal", __FILE__, __LINE__, __FUNCTION__);

        size_t i = 0;
        for ( ; it != itEnd; ++it, ++i)
        {
            TestHelpers::assertEqual(*it, queue[i], "iterators, push_back, non-full queue, value mismatch", __FILE__, __LINE__, __FUNCTION__);
        }

        TestHelpers::assertEqual(i, queue.size(), "iterators, push_back, non-full queue, size mismatch", __FILE__, __LINE__, __FUNCTION__);

        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "	FAILED: " << e.what()  << std::endl;
    }

    // iterators, push_back, full queue
    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        HeapCircularQueue<int> queue(2);

        queue.push_back(1);
        queue.push_back(2);

        HeapCircularQueue<int>::iterator it = queue.begin();
        HeapCircularQueue<int>::iterator itEnd = queue.end();

        TestHelpers::assertEqual(it != itEnd, true, "iterators, push_back, full queue, begin and end should not be equal", __FILE__, __LINE__, __FUNCTION__);

        size_t i = 0;
        for ( ; it != itEnd; ++it, ++i)
        {
            TestHelpers::assertEqual(*it, queue[i], "iterators, push_back, full queue, value mismatch", __FILE__, __LINE__, __FUNCTION__);
        }

        TestHelpers::assertEqual(i, queue.size(), "iterators, push_back, full queue, size mismatch", __FILE__, __LINE__, __FUNCTION__);

        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "	FAILED: " << e.what()  << std::endl;
    }

    // iterators, push_front, non-full queue
    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        HeapCircularQueue<int> queue(10);

        queue.push_front(1);
        queue.push_front(2);

        HeapCircularQueue<int>::iterator it = queue.begin();
        HeapCircularQueue<int>::iterator itEnd = queue.end();

        TestHelpers::assertEqual(it != itEnd, true, "iterators, push_front, non-full queue, begin and end should not be equal", __FILE__, __LINE__, __FUNCTION__);

        size_t i = 0;
        for ( ; it != itEnd; ++it, ++i)
        {
            TestHelpers::assertEqual(*it, queue[i], "iterators, push_front, non-full queue, value mismatch", __FILE__, __LINE__, __FUNCTION__);
        }

        TestHelpers::assertEqual(i, queue.size(), "iterators, push_front, non-full queue, size mismatch", __FILE__, __LINE__, __FUNCTION__);

        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "	FAILED: " << e.what()  << std::endl;
    }

//####################################################################################################
 std::cout << "TEST " << testNumber++ << ": ";
try
{
    HeapCircularQueue<int> queue(2);

    queue.push_front(1);
    queue.push_front(2);

    HeapCircularQueue<int>::iterator it = queue.begin();
    HeapCircularQueue<int>::iterator itEnd = queue.end();

    TestHelpers::assertEqual(it != itEnd, true, "iterators, push_front, non-full queue, begin and end should not be equal", __FILE__, __LINE__, __FUNCTION__);

    size_t i = 0;
    for ( ; it != itEnd; ++it, ++i)
    {
        TestHelpers::assertEqual(*it, queue[i], "iterators, push_front, non-full queue, value mismatch", __FILE__, __LINE__, __FUNCTION__);
    }

    TestHelpers::assertEqual(i, queue.size(), "iterators, push_front, non-full queue, size mismatch", __FILE__, __LINE__, __FUNCTION__);

    std::cout << "	PASSED" << std::endl;
}
catch (const std::exception& e)
{
    std::cout << "	FAILED: " << e.what()  << std::endl;
}

// copy constructor, full copy-from
std::cout << "TEST " << testNumber++ << ": ";
try
{
    HeapCircularQueue<int> queue(2);

    queue.push_back(1);
    queue.push_back(2);

    HeapCircularQueue<int> copy(queue);

    TestHelpers::assertEqual(queue.size(), copy.size(), "copy constructor, full copy-from, size mismatch", __FILE__, __LINE__, __FUNCTION__);

    HeapCircularQueue<int>::iterator itOriginal = queue.begin();
    HeapCircularQueue<int>::iterator itCopy = copy.begin();

    for ( ; itOriginal != queue.end() && itCopy != copy.end(); ++itOriginal, ++itCopy)
    {
        TestHelpers::assertEqual(*itOriginal, *itCopy, "copy constructor, full copy-from, value mismatch", __FILE__, __LINE__, __FUNCTION__);
    }

    std::cout << "	PASSED" << std::endl;
}
catch (const std::exception& e)
{
    std::cout << "	FAILED: " << e.what()  << std::endl;
}

// copy constructor, not-full copy-from
std::cout << "TEST " << testNumber++ << ": ";
try
{
    HeapCircularQueue<int> queue(10);

    queue.push_back(1);
    queue.push_back(2);

    HeapCircularQueue<int> copy(queue);

    TestHelpers::assertEqual(queue.size(), copy.size(), "copy constructor, not-full copy-from, size mismatch", __FILE__, __LINE__, __FUNCTION__);

    HeapCircularQueue<int>::iterator itOriginal = queue.begin();
    HeapCircularQueue<int>::iterator itCopy = copy.begin();

    for ( ; itOriginal != queue.end() && itCopy != copy.end(); ++itOriginal, ++itCopy)
    {
        TestHelpers::assertEqual(*itOriginal, *itCopy, "copy constructor, not-full copy-from value mismatch", __FILE__, __LINE__, __FUNCTION__);
    }

    std::cout << "	PASSED" << std::endl;
}
catch (const std::exception& e)
{
    std::cout << "	FAILED: " << e.what()  << std::endl;
}

// copy constructor, empty copy-from
std::cout << "TEST " << testNumber++ << ": ";
try
{
    HeapCircularQueue<int> queue(10);

    HeapCircularQueue<int> copy(queue);

    TestHelpers::assertEqual(queue.size(), copy.size(), "copy constructor, empty copy-from, size mismatch", __FILE__, __LINE__, __FUNCTION__);

    HeapCircularQueue<int>::iterator itOriginal = queue.begin();
    HeapCircularQueue<int>::iterator itCopy = copy.begin();

    for ( ; itOriginal != queue.end() && itCopy != copy.end(); ++itOriginal, ++itCopy)
    {
        TestHelpers::assertEqual(*itOriginal, *itCopy, "copy constructor, empty copy-from value mismatch", __FILE__, __LINE__, __FUNCTION__);
    }

    std::cout << "	PASSED" << std::endl;
}
catch (const std::exception& e)
{
    std::cout << "	FAILED: " << e.what()  << std::endl;
}

// copy assignment, full copy-from, empty copy-to
std::cout << "TEST " << testNumber++ << ": ";
try
{
    HeapCircularQueue<int> queue(2);

    queue.push_back(1);
    queue.push_back(2);

    HeapCircularQueue<int> assign(2);

    assign = queue;

    TestHelpers::assertEqual(queue.size(), assign.size(), "assignment, full copy-from, empty copy-to, size mismatch", __FILE__, __LINE__, __FUNCTION__);

    HeapCircularQueue<int>::iterator itOriginal = queue.begin();
    HeapCircularQueue<int>::iterator itCopy = assign.begin();

    for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
    {
        TestHelpers::assertEqual(*itOriginal, *itCopy, "assignment, full copy-from, empty copy-to, value mismatch", __FILE__, __LINE__, __FUNCTION__);
    }

    std::cout << "	PASSED" << std::endl;
}
catch (const std::exception& e)
{
    std::cout << "	FAILED: " << e.what()  << std::endl;
}

// copy assignment, non-full copy-from, empty copy-to
std::cout << "TEST " << testNumber++ << ": ";
try
{
    HeapCircularQueue<int> queue(10);

    queue.push_back(1);
    queue.push_back(2);

    HeapCircularQueue<int> assign(10);

    assign = queue;

    TestHelpers::assertEqual(queue.size(), assign.size(), "assignment, non-full copy-from, empty copy-to, size mismatch", __FILE__, __LINE__, __FUNCTION__);

    HeapCircularQueue<int>::iterator itOriginal = queue.begin();
    HeapCircularQueue<int>::iterator itCopy = assign.begin();

    for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
    {
        TestHelpers::assertEqual(*itOriginal, *itCopy, "assignment, non-full copy-from, empty copy-to, value mismatch", __FILE__, __LINE__, __FUNCTION__);
    }

    std::cout << "	PASSED" << std::endl;
}
catch (const std::exception& e)
{
    std::cout << "	FAILED: " << e.what()  << std::endl;
}

// copy assignment, empty copy-from, empty copy-to
std::cout << "TEST " << testNumber++ << ": ";
try
{
    HeapCircularQueue<int> queue(10);
    HeapCircularQueue<int> assign(10);

    assign = queue;

    TestHelpers::assertEqual(queue.size(), assign.size(), "assignment, empty copy-from, empty copy-to, size mismatch", __FILE__, __LINE__, __FUNCTION__);

    HeapCircularQueue<int>::iterator itOriginal = queue.begin();
    HeapCircularQueue<int>::iterator itCopy = assign.begin();

    for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
    {
        TestHelpers::assertEqual(*itOriginal, *itCopy, "assignment, empty copy-from, empty copy-to, value mismatch", __FILE__, __LINE__, __FUNCTION__);
    }

    std::cout << "	PASSED" << std::endl;
}
catch (const std::exception& e)
{
    std::cout << "	FAILED: " << e.what()  << std::endl;
}

// copy assignment, full copy-from, non-empty copy-to
std::cout << "TEST " << testNumber++ << ": ";
try
{
    HeapCircularQueue<int> queue(2);

    queue.push_back(1);
    queue.push_back(2);

    HeapCircularQueue<int> assign(2);
    assign.push_back(123);

    assign = queue;

    TestHelpers::assertEqual(queue.size(), assign.size(), "assignment, full copy-from, non-empty copy-to, size mismatch", __FILE__, __LINE__, __FUNCTION__);

    HeapCircularQueue<int>::iterator itOriginal = queue.begin();
    HeapCircularQueue<int>::iterator itCopy = assign.begin();

    for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
    {
        TestHelpers::assertEqual(*itOriginal, *itCopy, "assignment, full copy-from, non-empty copy-to, value mismatch", __FILE__, __LINE__, __FUNCTION__);
    }

    std::cout << "	PASSED" << std::endl;
}
catch (const std::exception& e)
{
    std::cout << "	FAILED: " << e.what()  << std::endl;
}

// copy assignment, non-full copy-from, non-empty copy-to
std::cout << "TEST " << testNumber++ << ": ";
try
{
    HeapCircularQueue<int> queue(10);

    queue.push_back(1);
    queue.push_back(2);

    HeapCircularQueue<int> assign(10);
    assign.push_back(123);

    assign = queue;

    TestHelpers::assertEqual(queue.size(), assign.size(), "assignment, non-full copy-from, non-empty copy-to, size mismatch", __FILE__, __LINE__, __FUNCTION__);

    HeapCircularQueue<int>::iterator itOriginal = queue.begin();
    HeapCircularQueue<int>::iterator itCopy = assign.begin();

    for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
    {
        TestHelpers::assertEqual(*itOriginal, *itCopy, "assignment, non-full copy-from, non-empty copy-to, value mismatch", __FILE__, __LINE__, __FUNCTION__);
    }

    std::cout << "	PASSED" << std::endl;
}
catch (const std::exception& e)
{
    std::cout << "	FAILED: " << e.what()  << std::endl;
}
    return (testNumber);
}

