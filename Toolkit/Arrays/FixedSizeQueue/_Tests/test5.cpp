/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test5.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 15:06:14 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 18:41:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <list>

// Project headers
# include "../FixedSizeQueue.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../_Tests/test.h"


int TestPart5(int testNumber)
{
    try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		FixedSizeQueue<int> 			queue(10);
        queue.push_back(5);
		queue.push_front(2);
        if (queue[0] != 2)
            throw std::logic_error("front should be 2");
		if (queue[1] != 5)
		{
			throw std::logic_error("back should be 5");
		}
		for (FixedSizeQueue<int>::iterator it = queue.begin(); it != queue.end(); ++it)
		{
			std::cout << *it << std::endl;
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}

    try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		FixedSizeQueue<int> 			queue(200);
		std::list<int> 					list;

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
			queue.push_back(i);

			list.push_front(i);
			queue.push_front(i);
		}
		if (list.size() != queue.size())
			throw std::logic_error("size mismatch");
		
		FixedSizeQueue<int>::iterator it = queue.begin();
		std::list<int>::iterator iter = list.begin();

		for ( ; it != queue.end() && iter != list.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}


    return (testNumber);
}