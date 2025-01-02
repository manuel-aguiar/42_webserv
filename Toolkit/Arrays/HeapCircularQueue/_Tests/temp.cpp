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
        
        resultInsertion = queue.push_front(frontNumber);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + "but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        /************* */
               
        resultInsertion = queue.push_front(backNumber);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + "but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue[0] != frontNumber)
            throw std::logic_error("index 0 is: " + to_string(queue[0]) + " but should be: " + to_string(frontNumber) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		if (queue[1] != backNumber)
			throw std::logic_error("index 1 is: " + to_string(queue[1]) + " but should be: " + to_string(frontNumber) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        if (queue.size() != expectedElemCount)
            throw std::logic_error("size should be: " + to_string(expectedElemCount) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        if (queue.back() != backNumber)
            throw std::logic_error("back should be: " + to_string(backNumber) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        if (queue.front() != frontNumber)
            throw std::logic_error("front should be: " + to_string(frontNumber) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


        resultInsertion = queue.push_front(10);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + "but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        /************* */
       
        resultInsertion = queue.push_front(10);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + "but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

 
        if (queue.size() != expectedElemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + to_string(queue.size()) + ", should be: " + to_string(expectedElemCount) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }  

        /************* */
        
        resultInsertion = queue.pop_front();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + "but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + to_string(queue.size()) + ", should be: " + to_string(expectedElemCount) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }  

        /************* */

        resultInsertion = queue.pop_front();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + "but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + to_string(queue.size()) + ", should be: " + to_string(expectedElemCount) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }   
        

        /************* */

        resultInsertion = queue.pop_front();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + "but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

    /************* */

        resultInsertion = queue.pop_front();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + "but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


        /************* */

        resultInsertion = queue.push_front(10);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += resultInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + "but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + to_string(queue.size()) + ", should be: " + to_string(expectedElemCount) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        } 

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}