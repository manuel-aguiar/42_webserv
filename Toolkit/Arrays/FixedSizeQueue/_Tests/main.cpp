/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 08:40:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 18:39:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

// Project headers
# include "../FixedSizeQueue.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"

# include "../../../_Tests/test.h"

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);
extern int TestPart3(int testNumber);
extern int TestPart4(int testNumber);
extern int TestPart5(int testNumber);

int ThisTest(int testNumber)
{

/******************************************************************** */

    try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::vector<int> 		std;
		FixedSizeQueue<int> 			queue(200);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.emplace_back(i);
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch");

		FixedSizeQueue<int>::iterator it = queue.begin();

        FixedSizeQueue<int> assign(200);
		assign.push_front(123);
		assign.push_front(456);
        assign = queue;

		FixedSizeQueue<int>::iterator itAssign = assign.begin();
        it = queue.begin();
        
		std::cout << "testing copy assignment" << std::endl;
        for ( ; it != queue.end() && itAssign != assign.end(); ++it, ++itAssign)
        {
			std::cout << "queue: " << *it << " assign: " << *itAssign << std::endl;
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

    return (testNumber);
}

int main()
{
    int testNumber = 1;
    
	std::cout << "\n*************** FixedSizeQueue tests ***************" << std::endl;
	
    //testNumber = ThisTest(testNumber);
    //testNumber = TestPart3(testNumber);
    //testNumber = TestPart4(testNumber);
    testNumber = TestPart5(testNumber);

	std::cout << "*********************************************\n" << std::endl;

    return (0);
}

/*
    c++ -Wall -Wextra -Werror -std=c++98 -g main.cpp ../../../MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.cpp ../../../MemoryPool/Nginx_MemoryPool/Nginx_MPool_Block.cpp -o main && ./main
*/
