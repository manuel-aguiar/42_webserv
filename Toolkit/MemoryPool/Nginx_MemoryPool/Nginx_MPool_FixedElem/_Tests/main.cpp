/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:41:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/27 16:44:09 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//C++ headers
#include <iostream>

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);

int main(void)
{
    int testNumber = 1;

	std::cout << "\n*************** Nginx_MPool_FixedElem tests ***************" << std::endl;
	
    testNumber = TestPart1(testNumber);
	testNumber = TestPart2(testNumber);

	std::cout << "**********************************************************\n" << std::endl;

    return 0;
}

