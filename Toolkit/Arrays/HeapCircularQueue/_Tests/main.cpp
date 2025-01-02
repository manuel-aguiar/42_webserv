/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 08:40:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 23:13:51 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

# include "../../../_Tests/test.h"

//extern int TestPart1(int testNumber);
//extern int TestPart2(int testNumber);
//extern int TestPart3(int testNumber);
//extern int TestPart4(int testNumber);
//extern int TestPart5(int testNumber);

# include <sstream>
template <typename T>
std::string to_string(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

std::string FileLineFunction(const char* file, const int line, const char* function)
{
    //ultra reallocs F iiiitttttt
    return std::string("\tFile: ") + file + "\n\tLine: " + to_string(line) + "\n\tFunction: " + function + '\n';
}

int ThisTest(int testNumber)
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
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
            throw std::logic_error("size is " + to_string(queue.size()) + " but should be: " + to_string(expectedElemCount) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        /************* */
               
        resultInsertion = queue.push_back(backNumber);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
            throw std::logic_error("size is " + to_string(queue.size()) + " but should be: " + to_string(expectedElemCount) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        /************* */

        if (queue[0] != frontNumber)
            throw std::logic_error("index 0 is: " + to_string(queue[0]) + " but should be: " + to_string(frontNumber) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		
        if (queue[1] != backNumber)
			throw std::logic_error("index 1 is: " + to_string(queue[1]) + " but should be: " + to_string(frontNumber) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        if (queue.size() != expectedElemCount)
            throw std::logic_error("size is " + to_string(queue.size()) + " but should be: " + to_string(expectedElemCount) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        if (queue.back() != backNumber)
            throw std::logic_error("back is " + to_string(queue.back()) + " but should be: " + to_string(backNumber) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        if (queue.front() != frontNumber)
            throw std::logic_error("back is " + to_string(queue.front()) + " but should be: " + to_string(backNumber) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));



        resultInsertion = queue.push_back(10);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        /************* */
       
        resultInsertion = queue.push_back(10);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

 
        if (queue.size() != expectedElemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + to_string(queue.size()) + ", should be: " + to_string(expectedElemCount) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }  

        /************* */
        
        resultInsertion = queue.pop_back();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + to_string(queue.size()) + ", should be: " + to_string(expectedElemCount) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }  

        /************* */

        resultInsertion = queue.pop_back();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
        {
            throw std::logic_error("size doesn't match, size is: " + to_string(queue.size()) + ", should be: " + to_string(expectedElemCount) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }   
        

        /************* */

        resultInsertion = queue.pop_back();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

    /************* */

        resultInsertion = queue.pop_back();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


        /************* */

        resultInsertion = queue.push_back(10);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += resultInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
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
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
            throw std::logic_error("size is " + to_string(queue.size()) + " but should be: " + to_string(expectedElemCount) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        /************* */
               
        resultInsertion = queue.push_front(frontNumber);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
            throw std::logic_error("size is " + to_string(queue.size()) + " but should be: " + to_string(expectedElemCount) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        /************* */

        if (queue[0] != frontNumber)
            throw std::logic_error("index 0 is: " + to_string(queue[0]) + " but should be: " + to_string(frontNumber) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		
        
        if (queue[1] != backNumber)
			throw std::logic_error("index 1 is: " + to_string(queue[1]) + " but should be: " + to_string(frontNumber) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        
        if (queue.size() != expectedElemCount)
            throw std::logic_error("size is " + to_string(queue.size()) + " but should be: " + to_string(expectedElemCount) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        
        if (queue.back() != backNumber)
            throw std::logic_error("back is " + to_string(queue.back()) + " but should be: " + to_string(backNumber) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        
        if (queue.front() != frontNumber)
            throw std::logic_error("back is " + to_string(queue.front()) + " but should be: " + to_string(backNumber) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


        resultInsertion = queue.push_front(10);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        /************* */
       
        resultInsertion = queue.push_front(10);

        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
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
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
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
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
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
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

    /************* */

        resultInsertion = queue.pop_front();

        expectedInsertion = (expectedElemCount > 0);
        expectedElemCount -= expectedInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


        /************* */

        resultInsertion = queue.push_front(10);
        expectedInsertion = (expectedElemCount < queueSize);
        expectedElemCount += resultInsertion;
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
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


/*
    try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::list<int> 		std;
		HeapCircularQueue<int> 			queue(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
            queue.push_back(i);
		}

		if (std.size() != queue.size())
			throw std::logic_error("size mismatch");

        HeapCircularQueue<int>::iterator itQueue = queue.begin();
        std::list<int>::iterator itList = std.begin();
        for ( ; itQueue != queue.end() && itList != std.end(); ++itQueue, ++itList)
        {
            if (*itQueue != *itList)
                throw std::logic_error("value mismatch");
        }

        HeapCircularQueue<int> assign(100);
        assign = queue;

		HeapCircularQueue<int>::iterator it = queue.begin();
		HeapCircularQueue<int>::iterator itAssign = assign.begin();
		std::cout << "testing copy assignment" << std::endl;
        
        for ( ; it != queue.end() && itAssign != assign.end(); ++it, ++itAssign)
        {
            if (*it != *itAssign)
                throw std::logic_error("copy assignment, value mismatch");
        }

        if (queue.size() != assign.size())
			throw std::logic_error("copy assignment, size mismatch");


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
    testNumber++;
*/

/******************************************************************** */
/*
    try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::list<int> 		std;
		HeapCircularQueue<int> 			queue(200);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
            std.push_back(i);
            std.pop_front();
            std.push_front(i);
            std.pop_back();

			queue.push_back(i);
            queue.push_back(i);
            queue.pop_front();
            queue.push_front(i);
            queue.pop_back();
		}

		for (int i = 0; i < 50; ++i)
		{
            std.pop_front();
            queue.pop_front();
		}

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
            std.push_back(i);
            std.pop_front();
            std.push_front(i);
            std.pop_back();

			queue.push_back(i);
            queue.push_back(i);
            queue.pop_front();
            queue.push_front(i);
            queue.pop_back();
		}

		if (std.size() != queue.size())
			throw std::logic_error("size mismatch");

        HeapCircularQueue<int>::iterator itQueue = queue.begin();
        std::list<int>::iterator itList = std.begin();
        for ( ; itQueue != queue.end() && itList != std.end(); ++itQueue, ++itList)
        {
            if (*itQueue != *itList)
                throw std::logic_error("value mismatch");
        }

        HeapCircularQueue<int> assign(200);
        assign = queue;

		HeapCircularQueue<int>::iterator it = queue.begin();
		HeapCircularQueue<int>::iterator itAssign = assign.begin();
		std::cout << "testing copy assignment" << std::endl;
        
        for ( ; it != queue.end() && itAssign != assign.end(); ++it, ++itAssign)
        {
            if (*it != *itAssign)
                throw std::logic_error("copy assignment, value mismatch");
        }

        if (queue.size() != assign.size())
			throw std::logic_error("copy assignment, size mismatch");


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
    testNumber++;
*/
    return (testNumber);
}

int main()
{
    int testNumber = 1;
    
	std::cout << "\n*************** HeapCircularQueue tests ***************" << std::endl;
	
    testNumber = ThisTest(testNumber);
    //testNumber = TestPart3(testNumber);
    //testNumber = TestPart4(testNumber);
    //testNumber = TestPart5(testNumber);

	std::cout << "*********************************************\n" << std::endl;

    return (0);
}

/*
    c++ -Wall -Wextra -Werror -std=c++98 -g main.cpp ../../../MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.cpp ../../../MemoryPool/Nginx_MemoryPool/Nginx_MPool_Block.cpp -o main && ./main
*/
