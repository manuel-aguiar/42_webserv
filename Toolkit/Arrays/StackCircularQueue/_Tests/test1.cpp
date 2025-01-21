

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

# include "../../../_Tests/TestHelpers.h"

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

		StackCircularQueue<int, queueSize> 			queue;

        /************* */
        
        resultInsertion = queue.push_back(frontNumber);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
            throw std::logic_error("size is " + TestHelpers::to_string(queue.size()) + " but should be: " + TestHelpers::to_string(expectedElemCount) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        /************* */
               
        resultInsertion = queue.emplace_back(backNumber);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
            throw std::logic_error("size is " + TestHelpers::to_string(queue.size()) + " but should be: " + TestHelpers::to_string(expectedElemCount) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        /************* */

        if (queue[0] != frontNumber)
            throw std::logic_error("index 0 is: " + TestHelpers::to_string(queue[0]) + " but should be: " + TestHelpers::to_string(frontNumber) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		
        if (queue[1] != backNumber)
			throw std::logic_error("index 1 is: " + TestHelpers::to_string(queue[1]) + " but should be: " + TestHelpers::to_string(frontNumber) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        if (queue.size() != expectedElemCount)
            throw std::logic_error("size is " + TestHelpers::to_string(queue.size()) + " but should be: " + TestHelpers::to_string(expectedElemCount) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        if (queue.back() != backNumber)
            throw std::logic_error("back is " + TestHelpers::to_string(queue.back()) + " but should be: " + TestHelpers::to_string(backNumber) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        if (queue.front() != frontNumber)
            throw std::logic_error("back is " + TestHelpers::to_string(queue.front()) + " but should be: " + TestHelpers::to_string(backNumber) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));



        resultInsertion = queue.push_back(10);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        /************* */
       
        resultInsertion = queue.push_back(10);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

 
        if (queue.size() != expectedElemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + TestHelpers::to_string(queue.size()) + ", should be: " + TestHelpers::to_string(expectedElemCount) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }  

        /************* */
        
        resultInsertion = queue.pop_back();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + TestHelpers::to_string(queue.size()) + ", should be: " + TestHelpers::to_string(expectedElemCount) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }  

        /************* */

        resultInsertion = queue.pop_back();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + TestHelpers::to_string(queue.size()) + ", should be: " + TestHelpers::to_string(expectedElemCount) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }   
        

        /************* */

        resultInsertion = queue.pop_back();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

    /************* */

        resultInsertion = queue.pop_back();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


        /************* */

        resultInsertion = queue.push_back(10);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += resultInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + TestHelpers::to_string(queue.size()) + ", should be: " + TestHelpers::to_string(expectedElemCount) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        } 

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}




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

		StackCircularQueue<int, queueSize> 			queue;

        /************* */
        
        resultInsertion = queue.push_front(backNumber);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
            throw std::logic_error("size is " + TestHelpers::to_string(queue.size()) + " but should be: " + TestHelpers::to_string(expectedElemCount) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        /************* */
               
        resultInsertion = queue.push_front(frontNumber);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
            throw std::logic_error("size is " + TestHelpers::to_string(queue.size()) + " but should be: " + TestHelpers::to_string(expectedElemCount) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        /************* */

        if (queue[0] != frontNumber)
            throw std::logic_error("index 0 is: " + TestHelpers::to_string(queue[0]) + " but should be: " + TestHelpers::to_string(frontNumber) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		
        
        if (queue[1] != backNumber)
			throw std::logic_error("index 1 is: " + TestHelpers::to_string(queue[1]) + " but should be: " + TestHelpers::to_string(frontNumber) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        
        if (queue.size() != expectedElemCount)
            throw std::logic_error("size is " + TestHelpers::to_string(queue.size()) + " but should be: " + TestHelpers::to_string(expectedElemCount) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        
        if (queue.back() != backNumber)
            throw std::logic_error("back is " + TestHelpers::to_string(queue.back()) + " but should be: " + TestHelpers::to_string(backNumber) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        
        if (queue.front() != frontNumber)
            throw std::logic_error("back is " + TestHelpers::to_string(queue.front()) + " but should be: " + TestHelpers::to_string(backNumber) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


        resultInsertion = queue.push_front(10);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        /************* */
       
        resultInsertion = queue.push_front(10);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

 
        if (queue.size() != expectedElemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + TestHelpers::to_string(queue.size()) + ", should be: " + TestHelpers::to_string(expectedElemCount) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }  

        /************* */
        
        resultInsertion = queue.pop_front();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + TestHelpers::to_string(queue.size()) + ", should be: " + TestHelpers::to_string(expectedElemCount) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }  

        /************* */

        resultInsertion = queue.pop_front();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + TestHelpers::to_string(queue.size()) + ", should be: " + TestHelpers::to_string(expectedElemCount) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }   
        

        /************* */

        resultInsertion = queue.pop_front();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

    /************* */

        resultInsertion = queue.pop_front();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


        /************* */

        resultInsertion = queue.push_front(10);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += resultInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + TestHelpers::to_string(resultInsertion) + " but expected: " + TestHelpers::to_string(expectedInsertion) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + TestHelpers::to_string(queue.size()) + ", should be: " + TestHelpers::to_string(expectedElemCount) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        } 

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
        const int queueSize = 10;
        StackCircularQueue<int, queueSize> queue;

        StackCircularQueue<int, queueSize>::iterator it = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itEnd = queue.end();

        if (it != itEnd)
            throw std::logic_error("iterators, empty queue, begin and end should be equal" + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

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
        const int queueSize = 10;
        StackCircularQueue<int, queueSize> queue;

        queue.push_back(1);
        queue.push_back(2);

        StackCircularQueue<int, queueSize>::iterator it = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itEnd = queue.end();

        if (it == itEnd)
            throw std::logic_error("iterators, push_back, non-full queue, begin and end should not be equal" + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        size_t i = 0;
        for ( ; it != itEnd; ++it, ++i)
        {
            if (*it != queue[i])
                throw std::logic_error("iterators, push_back, non-full queue, value mismatch, got " + TestHelpers::to_string(*it) + " expected: " + TestHelpers::to_string(queue[i]) + '\n'
                + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

        if (i != queue.size())
            throw std::logic_error("iterators, push_back, non-full queue, size mismatch, got " + TestHelpers::to_string(i) + " expected: " + TestHelpers::to_string(queue.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

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
        const int queueSize = 2;
        StackCircularQueue<int, queueSize> queue;

        queue.push_back(1);
        queue.push_back(2);

        StackCircularQueue<int, queueSize>::iterator it = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itEnd = queue.end();

        if (it == itEnd)
            throw std::logic_error("iterators, push_back, non-full queue, begin and end should not be equal" + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        size_t i = 0;
        for ( ; it != itEnd; ++it, ++i)
        {
            if (*it != queue[i])
                throw std::logic_error("iterators, push_back, non-full queue, value mismatch, got " + TestHelpers::to_string(*it) + " expected: " + TestHelpers::to_string(queue[i]) + '\n'
                + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

        if (i != queue.size())
            throw std::logic_error("iterators, push_back, non-full queue, size mismatch, got " + TestHelpers::to_string(i) + " expected: " + TestHelpers::to_string(queue.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

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
        const int queueSize = 10;
        StackCircularQueue<int, queueSize> queue;

        queue.push_front(1);
        queue.push_front(2);

        StackCircularQueue<int, queueSize>::iterator it = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itEnd = queue.end();
        if (it == itEnd)
            throw std::logic_error("iterators, push_front, non-full queue, begin and end should not be equal" + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        size_t i = 0;
        for ( ; it != itEnd; ++it, ++i)
        {
            if (*it != queue[i])
                throw std::logic_error("iterators, push_front, non-full queue, value mismatch, got " + TestHelpers::to_string(*it) + " expected: " + TestHelpers::to_string(queue[i]) + '\n'
                + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

        if (i != queue.size())
            throw std::logic_error("iterators, push_front, non-full queue, size mismatch, got " + TestHelpers::to_string(i) + " expected: " + TestHelpers::to_string(queue.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "	FAILED: " << e.what()  << std::endl;
    }

    // iterators, push_front, full queue
    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        const int queueSize = 2;
        StackCircularQueue<int, queueSize> queue;

        queue.push_front(1);
        queue.push_front(2);

        StackCircularQueue<int, queueSize>::iterator it = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itEnd = queue.end();

        if (it == itEnd)
            throw std::logic_error("iterators, push_front, non-full queue, begin and end should not be equal" + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        size_t i = 0;
        for ( ; it != itEnd; ++it, ++i)
        {
            if (*it != queue[i])
                throw std::logic_error("iterators, push_front, non-full queue, value mismatch, got " + TestHelpers::to_string(*it) + " expected: " + TestHelpers::to_string(queue[i]) + '\n'
                + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

        if (i != queue.size())
            throw std::logic_error("iterators, push_front, non-full queue, size mismatch, got " + TestHelpers::to_string(i) + " expected: " + TestHelpers::to_string(queue.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "	FAILED: " << e.what()  << std::endl;
    }

    //copy constructor, full copy-from
    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        const int queueSize = 2;
        StackCircularQueue<int, queueSize> queue;

        queue.push_back(1);
        queue.push_back(2);

        StackCircularQueue<int, queueSize> copy(queue);

        if (queue.size() != copy.size())
            throw std::logic_error("copy constructor, full copy-from, size mismatch, got " + TestHelpers::to_string(copy.size()) + " expected: " + TestHelpers::to_string(queue.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = copy.begin();

        for ( ; itOriginal != queue.end() && itCopy != copy.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("copy constructor, full copy-from, value mismatch, got " + TestHelpers::to_string(*itCopy) + " expected: " + TestHelpers::to_string(*itOriginal) + '\n'
                + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    //copy constructor, not-full copy-from
    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        const int queueSize = 10;
        StackCircularQueue<int, queueSize> queue;

        queue.push_back(1);
        queue.push_back(2);

        StackCircularQueue<int, queueSize> copy(queue);

        if (queue.size() != copy.size())
            throw std::logic_error("copy constructor, not-full copy-from, size mismatch, got " + TestHelpers::to_string(copy.size()) + " expected: " + TestHelpers::to_string(queue.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = copy.begin();

        for ( ; itOriginal != queue.end() && itCopy != copy.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("copy constructor, not-full copy-from value mismatch, got " + TestHelpers::to_string(*itCopy) + " expected: " + TestHelpers::to_string(*itOriginal) + '\n'
                + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    //copy constructor, empty copy-from
    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        const int queueSize = 10;
        StackCircularQueue<int, queueSize> queue;

        StackCircularQueue<int, queueSize> copy(queue);

        if (queue.size() != copy.size())
            throw std::logic_error("copy constructor, empty copy-from, size mismatch, got " + TestHelpers::to_string(copy.size()) + " expected: " + TestHelpers::to_string(queue.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = copy.begin();

        for ( ; itOriginal != queue.end() && itCopy != copy.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("copy constructor, empty copy-from value mismatch, got " + TestHelpers::to_string(*itCopy) + " expected: " + TestHelpers::to_string(*itOriginal) + '\n'
                + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

     //copy assignment, full copy-from, empty copy-to
    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        const int queueSize = 2;
        StackCircularQueue<int, queueSize> queue;

        queue.push_back(1);
        queue.push_back(2);

        StackCircularQueue<int, queueSize> assign(3);   //initalize elements to '3'

        assign = queue;

        if (queue.size() != assign.size())
            throw std::logic_error("assignment, full copy-from, empty copy-to, size mismatch, got " + TestHelpers::to_string(assign.size()) + " expected: " + TestHelpers::to_string(queue.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("assignment, full copy-from, empty copy-to, value mismatch, got " + TestHelpers::to_string(*itCopy) + " expected: " + TestHelpers::to_string(*itOriginal) + '\n'
                + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

 //copy assignment, non-full copy-from, empty copy-to
    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        const int queueSize = 10;
        StackCircularQueue<int, queueSize> queue(5); //initialize elements to 5

        queue.push_back(1);
        queue.push_back(2);

        StackCircularQueue<int, queueSize> assign(7); //initialize elements to 7

        assign = queue;

        if (queue.size() != assign.size())
            throw std::logic_error("assignment, non-full copy-from, empty copy-to, size mismatch, got " + TestHelpers::to_string(assign.size()) + " expected: " + TestHelpers::to_string(queue.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("assignment, non-full copy-from, empty copy-to, value mismatch, got " + TestHelpers::to_string(*itCopy) + " expected: " + TestHelpers::to_string(*itOriginal) + '\n'
                + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

 //copy assignment, empty copy-from, empty copy-to
    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        const int queueSize = 10;
        StackCircularQueue<int, queueSize> queue;
        StackCircularQueue<int, queueSize> assign;

        assign = queue;

        if (queue.size() != assign.size())
            throw std::logic_error("assignment, empty copy-from, empty copy-to, size mismatch, got " + TestHelpers::to_string(assign.size()) + " expected: " + TestHelpers::to_string(queue.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("assignment, empty copy-from, empty copy-to, value mismatch, got " + TestHelpers::to_string(*itCopy) + " expected: " + TestHelpers::to_string(*itOriginal) + '\n'
                + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

     //copy assignment, full copy-from, non-empty copy-to
    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        const int queueSize = 2;
        StackCircularQueue<int, queueSize> queue;

        queue.push_back(1);
        queue.push_back(2);

        StackCircularQueue<int, queueSize> assign;
        assign.push_back(123);

        assign = queue;

        if (queue.size() != assign.size())
            throw std::logic_error("assignment, full copy-from, non-empty copy-to, size mismatch, got " + TestHelpers::to_string(assign.size()) + " expected: " + TestHelpers::to_string(queue.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("assignment, full copy-from, non-empty copy-to, value mismatch, got " + TestHelpers::to_string(*itCopy) + " expected: " + TestHelpers::to_string(*itOriginal) + '\n'
                + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

     //copy assignment, non-full copy-from, non-empty copy-to
    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        const int queueSize = 10;
        StackCircularQueue<int, queueSize> queue;

        queue.push_back(1);
        queue.push_back(2);

        StackCircularQueue<int, queueSize> assign;
        assign.push_back(123);

        assign = queue;

        if (queue.size() != assign.size())
            throw std::logic_error("assignment, non-full copy-from, non-empty copy-to, size mismatch, got " + TestHelpers::to_string(assign.size()) + " expected: " + TestHelpers::to_string(queue.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("assignment, non-full copy-from, non-empty copy-to, value mismatch, got " + TestHelpers::to_string(*itCopy) + " expected: " + TestHelpers::to_string(*itOriginal) + '\n'
                + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

     //copy assignment, empty copy-from, non-empty copy-to
    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        const int queueSize = 10;
        StackCircularQueue<int, queueSize> queue;

        StackCircularQueue<int, queueSize> assign;
        assign.push_back(123);

        assign = queue;

        if (queue.size() != assign.size())
            throw std::logic_error("assignment, empty copy-from, non-empty copy-to, size mismatch, got " + TestHelpers::to_string(assign.size()) + " expected: " + TestHelpers::to_string(queue.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("assignment, empty copy-from, non-empty copy-to, value mismatch, got " + TestHelpers::to_string(*itCopy) + " expected: " + TestHelpers::to_string(*itOriginal) + '\n'
                + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

 //copy assignment, full copy-from, full copy-to
    std::cout << "TEST " << testNumber++ << ": ";
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

        if (queue.size() != assign.size())
            throw std::logic_error("assignment, full copy-from, full copy-to, size mismatch, got " + TestHelpers::to_string(assign.size()) + " expected: " + TestHelpers::to_string(queue.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("assignment, full copy-from, full copy-to, value mismatch, got " + TestHelpers::to_string(*itCopy) + " expected: " + TestHelpers::to_string(*itOriginal) + '\n'
                + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

 //copy assignment, non-full copy-from, full copy-to
    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        const int queueSize = 2;
        StackCircularQueue<int, queueSize> queue;

        queue.push_back(1);

        StackCircularQueue<int, queueSize> assign;

        assign.push_back(3);
        assign.push_back(4);

        assign = queue;

        if (queue.size() != assign.size())
            throw std::logic_error("assignment, non-full copy-from, full copy-to, size mismatch, got " + TestHelpers::to_string(assign.size()) + " expected: " + TestHelpers::to_string(queue.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("assignment, non-full copy-from, full copy-to, value mismatch, got " + TestHelpers::to_string(*itCopy) + " expected: " + TestHelpers::to_string(*itOriginal) + '\n'
                + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

 //copy assignment, empty copy-from, full copy-to
    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        const int queueSize = 2;
        StackCircularQueue<int, queueSize> queue;

        StackCircularQueue<int, queueSize> assign;

        assign.push_back(3);
        assign.push_back(4);

        assign = queue;

        if (queue.size() != assign.size())
            throw std::logic_error("assignment, empty copy-from, full copy-to, size mismatch, got " + TestHelpers::to_string(assign.size()) + " expected: " + TestHelpers::to_string(queue.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        StackCircularQueue<int, queueSize>::iterator itOriginal = queue.begin();
        StackCircularQueue<int, queueSize>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("assignment, empty copy-from, full copy-to, value mismatch, got " + TestHelpers::to_string(*itCopy) + " expected: " + TestHelpers::to_string(*itOriginal) + '\n'
                + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    return (testNumber);
}

