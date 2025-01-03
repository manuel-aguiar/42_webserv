/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 08:40:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/03 11:58:17 by mmaria-d         ###   ########.fr       */
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
        if (resultInsertion != expectedInsertion)
            throw std::logic_error("result was " + to_string(resultInsertion) + " but expected: " + to_string(expectedInsertion) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        if (queue.size() != expectedElemCount)
            throw std::logic_error("size is " + to_string(queue.size()) + " but should be: " + to_string(expectedElemCount) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        /************* */
               
        resultInsertion = queue.emplace_back(backNumber);

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


    //copy constructor, full copy-from
    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        HeapCircularQueue<int> queue(2);

        queue.push_back(1);
        queue.push_back(2);

        HeapCircularQueue<int> copy(queue);

        if (queue.size() != copy.size())
            throw std::logic_error("copy constructor, full copy-from, size mismatch, got " + to_string(copy.size()) + " expected: " + to_string(queue.size()) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        HeapCircularQueue<int>::iterator itOriginal = queue.begin();
        HeapCircularQueue<int>::iterator itCopy = copy.begin();

        for ( ; itOriginal != queue.end() && itCopy != copy.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("copy constructor, full copy-from, value mismatch, got " + to_string(*itCopy) + " expected: " + to_string(*itOriginal) + '\n'
                + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
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
        HeapCircularQueue<int> queue(10);

        queue.push_back(1);
        queue.push_back(2);

        HeapCircularQueue<int> copy(queue);

        if (queue.size() != copy.size())
            throw std::logic_error("copy constructor, not-full copy-from, size mismatch, got " + to_string(copy.size()) + " expected: " + to_string(queue.size()) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        HeapCircularQueue<int>::iterator itOriginal = queue.begin();
        HeapCircularQueue<int>::iterator itCopy = copy.begin();

        for ( ; itOriginal != queue.end() && itCopy != copy.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("copy constructor, not-full copy-from value mismatch, got " + to_string(*itCopy) + " expected: " + to_string(*itOriginal) + '\n'
                + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
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
        HeapCircularQueue<int> queue(2);

        queue.push_back(1);
        queue.push_back(2);

        HeapCircularQueue<int> assign(2);

        assign = queue;

        if (queue.size() != assign.size())
            throw std::logic_error("assignment, full copy-from, empty copy-to, size mismatch, got " + to_string(assign.size()) + " expected: " + to_string(queue.size()) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        HeapCircularQueue<int>::iterator itOriginal = queue.begin();
        HeapCircularQueue<int>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("assignment, full copy-from, empty copy-to, value mismatch, got " + to_string(*itCopy) + " expected: " + to_string(*itOriginal) + '\n'
                + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
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
        HeapCircularQueue<int> queue(10);

        queue.push_back(1);
        queue.push_back(2);

        HeapCircularQueue<int> assign(10);

        assign = queue;

        if (queue.size() != assign.size())
            throw std::logic_error("assignment, non-full copy-from, empty copy-to, size mismatch, got " + to_string(assign.size()) + " expected: " + to_string(queue.size()) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        HeapCircularQueue<int>::iterator itOriginal = queue.begin();
        HeapCircularQueue<int>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("assignment, non-full copy-from, empty copy-to, value mismatch, got " + to_string(*itCopy) + " expected: " + to_string(*itOriginal) + '\n'
                + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
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
        HeapCircularQueue<int> queue(2);

        queue.push_back(1);
        queue.push_back(2);

        HeapCircularQueue<int> assign(2);

        assign = queue;
        assign.push_back(123);

        if (queue.size() != assign.size())
            throw std::logic_error("assignment, full copy-from, non-empty copy-to, size mismatch, got " + to_string(assign.size()) + " expected: " + to_string(queue.size()) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        HeapCircularQueue<int>::iterator itOriginal = queue.begin();
        HeapCircularQueue<int>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("assignment, full copy-from, non-empty copy-to, value mismatch, got " + to_string(*itCopy) + " expected: " + to_string(*itOriginal) + '\n'
                + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
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
        HeapCircularQueue<int> queue(10);

        queue.push_back(1);
        queue.push_back(2);

        HeapCircularQueue<int> assign(10);

        assign = queue;
        assign.push_back(123);

        if (queue.size() != assign.size())
            throw std::logic_error("assignment, non-full copy-from, non-empty copy-to, size mismatch, got " + to_string(assign.size()) + " expected: " + to_string(queue.size()) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        HeapCircularQueue<int>::iterator itOriginal = queue.begin();
        HeapCircularQueue<int>::iterator itCopy = assign.begin();

        for ( ; itOriginal != queue.end() && itCopy != assign.end(); ++itOriginal, ++itCopy)
        {
            if (*itOriginal != *itCopy)
                throw std::logic_error("assignment, non-full copy-from, non-empty copy-to, value mismatch, got " + to_string(*itCopy) + " expected: " + to_string(*itOriginal) + '\n'
                + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    return (testNumber);
}

