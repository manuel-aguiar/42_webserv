/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_StressChildren.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:46:00 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/12 00:36:27 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include "CgiStressTest.hpp"

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);

int main(void)
{
	int testNumber = 1;

	std::cout << "\n*************** CgiModule Stress Children with Valgrind ***************" << std::endl;

    testNumber = TestPart1(testNumber);
    testNumber = TestPart2(testNumber);
	testNumber = CgiStressTest::StressTest(testNumber, 5, 50, 100, 2000, &CgiStressTest::AllInvalidCriteria, "AllInvalidCriteria");	
    testNumber = CgiStressTest::StressTest(testNumber, 5, 50, 100, 2000, &CgiStressTest::MixedCriteria, "MixedCriteria");	
    testNumber = CgiStressTest::StressTest(testNumber, 5, 50, 100, 2000, &CgiStressTest::AllValidCriteria, "AllValidCriteria");	
    std::cout << "\n*******************************************************************" << std::endl;
	return (0);
}