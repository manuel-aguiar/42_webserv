/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:41:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/05 23:19:16 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//C++ headers
#include <iostream>

extern int TestPart1(int testNumber);

int main(void)
{
    int testNumber = 1;

	std::cout << "\n*************** Nginx_MPool_FixedElem tests ***************" << std::endl;
	
    testNumber = TestPart1(testNumber);

	std::cout << "**********************************************************\n" << std::endl;

    return 0;
}

