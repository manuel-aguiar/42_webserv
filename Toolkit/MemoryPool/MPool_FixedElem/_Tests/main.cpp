/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 09:01:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/06 00:21:04 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MPool_FixedElem.hpp"
#include <list>

#include "../../../_Tests/test.h"

extern int TestPart1(int testNumber);

int main(void)
{
    int testNumber = 1;

	std::cout << "\n*************** MPool_FixedElem tests ***************" << std::endl;
	
    testNumber = TestPart1(testNumber);

	std::cout << "*****************************************************\n" << std::endl;

    return 0;
}