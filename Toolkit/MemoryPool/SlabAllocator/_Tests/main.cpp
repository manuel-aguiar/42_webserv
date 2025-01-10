/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:41:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/08 00:03:21 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//C++ headers
#include <iostream>

extern int TestPart1(int testNumber);

int main(void)
{
    int testNumber = 1;

	std::cout << "\n*************** FixedBlock_PoolAllocator tests ***************" << std::endl;
	
    testNumber = TestPart1(testNumber);

	std::cout << "**********************************************************\n" << std::endl;

    return 0;
}

