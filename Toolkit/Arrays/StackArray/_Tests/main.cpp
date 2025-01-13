/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 08:40:58 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 09:38:03 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// C++ headers
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

// Project headers
#include "../StackArray.hpp"
#include "../../../_Tests/ToolkitDummy.hpp"
#include "../../../_Tests/ToolkitBase.hpp"
#include "../../../_Tests/ToolkitDerived.hpp"

extern int TestPart1(int testNumber);
extern int StressTest(int testNumber);

int main(void)
{
	int testNumber = 1;
	std::cout << "\n*************** StackArray tests ***************" << std::endl;

	testNumber = TestPart1(testNumber);
	testNumber = StressTest(testNumber);

	std::cout << "*********************************************\n" << std::endl;
}