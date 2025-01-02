/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 09:21:17 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 11:31:44 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
#include "../../MemoryPool/MemoryPool.h"
#include "../List.hpp"
#include "../../_Tests/ToolkitDummy.hpp"

// C++ headers
#include <list>
#include <iostream>
#include <cstring>
#include <stdexcept>

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);


int main(void)
{
	int testNumber = 1;

	std::cout << "\n*************** List tests ***************" << std::endl;

	testNumber = TestPart1(testNumber);
	testNumber = TestPart2(testNumber);

	std::cout << "******************************************\n" << std::endl;

}