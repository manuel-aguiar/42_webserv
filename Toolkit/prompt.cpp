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

#####################################

i want you to apply the same logic to these following code: same TestHelpers::assertEqual prototype (come up with a small error message if there is none), no auto, all c++98, respect the formatting.

Here it is:



