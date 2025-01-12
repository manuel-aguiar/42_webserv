/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_StressValgrind.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:46:00 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/12 00:07:00 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include "CgiStressTest.hpp"

extern int TestPart3(int testNumber);

int main(void)
{
	int testNumber = 1;

	std::cout << "\n*************** CgiModule Stress Tests with Valgrind ***************" << std::endl;

	testNumber = CgiStressTest::StressTest(testNumber, 3, 3, 3, 500, &CgiStressTest::AllSuccessCriteria, "AllSuccessCriteria");
	testNumber = CgiStressTest::StressTest(testNumber, 5, 100, 100, 1500, &CgiStressTest::MixedCriteria, "MixedCriteria");	
	testNumber = CgiStressTest::StressTest(testNumber, 5, 50, 100, 2000, &CgiStressTest::MixedCriteria, "MixedCriteria");	
	testNumber = CgiStressTest::StressTest(testNumber, 5, 50, 100, 2000, &CgiStressTest::AllInvalidCriteria, "AllInvalidCriteria");	
	testNumber = CgiStressTest::StressTest(testNumber, 10, 50, 500, 2000, &CgiStressTest::MixedCriteria, "MixedCriteria");	

    std::cout << "\n*******************************************************************" << std::endl;
	return (0);
}