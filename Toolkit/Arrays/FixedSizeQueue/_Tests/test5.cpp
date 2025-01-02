/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test5.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 15:06:14 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 15:55:35 by mmaria-d         ###   ########.fr       */
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
			std::cout << "queue[1]: " << queue[1] << std::endl;
			throw std::logic_error("back should be 5");
		}
		
		FixedSizeQueue<int>::iterator it = queue.begin();
		std::cout << "queue: " << *it << std::endl;
		/*
		for ( ; it != queue.end(); ++it)
		{
			std::cout << "queue: " << *it << std::endl;
		}
		*/
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}



    return (testNumber);
}