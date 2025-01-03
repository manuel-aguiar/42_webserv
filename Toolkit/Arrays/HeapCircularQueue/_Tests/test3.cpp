/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test3.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 10:19:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/03 10:59:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <list>

// Project headers
# include "../HeapCircularQueue.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../_Tests/test.h"


int TestPart3(int testNumber)
{
    try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::list<ToolkitBase*> 				std;
		HeapCircularQueue<ToolkitBase*> 		queue(300);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(new ToolkitBase(i));
			queue.push_back(new ToolkitBase(i));

			std.push_front(new ToolkitDerived(i));
			queue.push_front(new ToolkitDerived(i));

			std.push_back(new ToolkitDerived(i));
			queue.push_back(new ToolkitDerived(i));

			delete std.back();
			delete queue.back();

			std.pop_back();
			queue.pop_back();
			std.push_front(new ToolkitBase(i));
			queue.push_front(new ToolkitBase(i));
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch, got " + to_string(queue.size()) + " expected: " + to_string(std.size())
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		HeapCircularQueue<ToolkitBase*>::iterator it = queue.begin();
		std::list<ToolkitBase*>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			if (**it != **iter)
				throw std::logic_error("value mismatch, got " + to_string((*it)->getValue()) + " expected: " + to_string((*iter)->getValue())
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

		it = queue.begin();
		iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			delete (*it);
			delete (*iter);
		}


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    return (testNumber);
}