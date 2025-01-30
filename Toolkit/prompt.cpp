/*

i am refactoring c++98 code with a new function i made. here is what i have done:

###############


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
		TEST_INTRO(testNumber++);

        const int queueSize = 10;
        const int frontNumber = 5;
        const int backNumber = 2;

        size_t expectedElemCount = 0;
        bool resultInsertion;
        bool expectedInsertion;

		HeapCircularQueue<int> 			queue(queueSize);

        
        resultInsertion = queue.push_back(frontNumber);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;

        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");

               
        resultInsertion = queue.emplace_back(backNumber);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;

        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");


        EXPECT_EQUAL(queue[0], frontNumber, "index 0 doesn't match ");
        EXPECT_EQUAL(queue[1], backNumber, "index 1 doesn't match ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");        
        EXPECT_EQUAL(queue.back(), backNumber, "back doesn't match ");
        EXPECT_EQUAL(queue.front(), frontNumber, "front doesn't match ");        

        resultInsertion = queue.push_back(10);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;

        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");

       
        resultInsertion = queue.push_back(10);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");

        
        resultInsertion = queue.pop_back();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");


        resultInsertion = queue.pop_back();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");


        resultInsertion = queue.pop_back();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");


        resultInsertion = queue.pop_back();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");


        resultInsertion = queue.push_back(10);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += resultInsertion;
        EXPECT_EQUAL(resultInsertion, expectedInsertion, "insertion result is not the same ");
        EXPECT_EQUAL(queue.size(), expectedElemCount, "size is not the same ");

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

#####################################

i want you to apply the same logic to these following code: same EXPECT_EQUAL prototype (come up with a small error message if there is none), no auto, all c++98, respect the formatting.

Here it is:

*/

