/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 08:40:58 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/06 23:36:29 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// C++ headers
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

// Project headers
#include "../StackSlotArray.hpp"
#include "../../../../_Tests/ToolkitDummy.hpp"
#include "../../../../_Tests/ToolkitBase.hpp"
#include "../../../../_Tests/ToolkitDerived.hpp"

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);

int main(void)
{
	int testNumber = 1;
	std::cout << "\n*************** StackArray tests ***************" << std::endl;

	std::cout << "TEST " << testNumber++ << ": ";
	try
	{
		const int arraySize = 100;
		StackSlotArray<ToolkitDummy, arraySize> array;

		ToolkitDummy* ptr = array.emplace(5);

		std::cout << "pointer is at: " << ptr << std::endl;

		array.destroy(ptr);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	std::cout << "*********************************************\n" << std::endl;
}