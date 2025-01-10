/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_StressTest.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:46:00 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 10:03:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

extern int Impl_StressTest(int testNumber, const int workers, const int backlog, const int connectionCount);

int main(void)
{
	int testNumber = 1;

	std::cout << "\n*************** CgiModule Stress Tests ***************" << std::endl;

	testNumber = Impl_StressTest(testNumber, 1, 1, 1);
	testNumber = Impl_StressTest(testNumber, 10, 100, 100);
	testNumber = Impl_StressTest(testNumber, 5, 100, 1000);

	testNumber = Impl_StressTest(testNumber, 50, 200, 1000);
	testNumber = Impl_StressTest(testNumber, 50, 200, 1000);
	testNumber = Impl_StressTest(testNumber, 50, 200, 1000);
	testNumber = Impl_StressTest(testNumber, 50, 200, 1000);
	testNumber = Impl_StressTest(testNumber, 50, 200, 1000);
	testNumber = Impl_StressTest(testNumber, 50, 200, 1000);
	testNumber = Impl_StressTest(testNumber, 50, 200, 1000);

	testNumber = Impl_StressTest(testNumber, 50, 500, 5000);
	testNumber = Impl_StressTest(testNumber, 50, 500, 5000);
	testNumber = Impl_StressTest(testNumber, 50, 500, 5000);
	testNumber = Impl_StressTest(testNumber, 50, 500, 5000);
	testNumber = Impl_StressTest(testNumber, 50, 500, 5000);
	testNumber = Impl_StressTest(testNumber, 50, 500, 5000);
	
	std::cout << "\n*************** ********************************" << std::endl;

	return (0);
}