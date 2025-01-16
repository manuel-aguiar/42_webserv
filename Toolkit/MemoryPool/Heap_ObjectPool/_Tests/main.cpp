/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 09:01:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 01:12:43 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Heap_ObjectPool.hpp"
#include <list>

#include "../../../_Tests/test.h"

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);

int main(void)
{
    int testNumber = 1;

	std::cout << "\n*************** Heap ObjectPool tests ***************" << std::endl;
	
    testNumber = TestPart1(testNumber);
    testNumber = TestPart2(testNumber);

	std::cout << "*****************************************************\n" << std::endl;

    return 0;
}