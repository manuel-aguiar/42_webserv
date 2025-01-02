int ThisTest(int testNumber)
{

    try
	{
		std::cout << "TEST " << testNumber++ << ": ";

        const int queueSize = 2;
        const int frontNumber = 5;
        const int backNumber = 2;
        size_t elemCount = 0;

		FixedSizeQueue<int> 			queue(queueSize);
        elemCount += (queue.push_front(frontNumber) == true);
		elemCount += (queue.push_front(backNumber) == true);

        if (queue[0] != frontNumber)
            throw std::logic_error("index 0 should be: " + to_string(frontNumber));
		if (queue[1] != backNumber)
			throw std::logic_error("index 1 should be: " + to_string(backNumber));
        if (queue.size() != elemCount)
            throw std::logic_error("size should be: " + to_string(elemCount));
        if (queue.back() != backNumber)
            throw std::logic_error("back should be: " + to_string(backNumber));
        if (queue.front() != frontNumber)
            throw std::logic_error("front should be: " + to_string(frontNumber));


        bool res = queue.push_front(10);
        elemCount += res;
        if (res != false)
            throw std::logic_error("push_front shoudl return false, queue is full");

       
        res = queue.push_front(10);
        elemCount += res;
        if (res != false)
            throw std::logic_error("second push_front shoudl return false, queue is full");

 
        if (queue.size() != elemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + to_string(queue.size()) + ", should be: " + to_string(elemCount));
        }  

        res = queue.pop_front();
        elemCount -= res;
        if (res != true)
            throw std::logic_error("pop_front shoudl return true, queue is not empty");

        if (queue.size() != elemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + to_string(queue.size()) + ", should be: " + to_string(elemCount));
        }  

        res = queue.pop_front();
        elemCount -= res;
        if (res != true)
            throw std::logic_error("pop_front shoudl return true, queue is not empty");

        if (queue.size() != elemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + to_string(queue.size()) + ", should be: " + to_string(elemCount));
        }   
            
        res = queue.pop_front();
        elemCount -= res;
        if (res != false)
            throw std::logic_error("pop_front shoudl return false, queue is empty");

        res = queue.pop_front();
        elemCount -= res;
        if (res != false)
            throw std::logic_error("pop_front shoudl return false, queue is empty");

        res = queue.push_front(10);
        elemCount += res;
        if (res != true)
            throw std::logic_error("push_front shoudl return true, queue is not full");

        if (queue.size() != elemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + to_string(queue.size()) + ", should be: " + to_string(elemCount));
        } 

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}