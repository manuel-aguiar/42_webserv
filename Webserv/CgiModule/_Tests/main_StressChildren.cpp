/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_StressChildren.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:46:00 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/14 16:56:47 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include "CgiStressTest.hpp"
# include <unistd.h>

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);

int main(void)
{
	int testNumber = 1;

	std::cout << "\n*************** CgiModule Stress Children with Valgrind ***************" << std::endl;

    testNumber = TestPart1(testNumber);
    testNumber = TestPart2(testNumber);
    testNumber = CgiStressTest::StressTest(testNumber, 5, 50, 100, 5000, &CgiStressTest::InvalidInterpreterPath, "InvalidInterpreterPath");	
    testNumber = CgiStressTest::StressTest(testNumber, 5, 50, 100, 5000, &CgiStressTest::InvalidInterpreter, "InvalidInterpreter");	
    //testNumber = CgiStressTest::StressTest(testNumber, 5, 50, 100, 5000, &CgiStressTest::InvalidScript, "InvalidScript");
	testNumber = CgiStressTest::StressTest(testNumber, 5, 50, 100, 5000, &CgiStressTest::AllInvalidCriteria, "AllInvalidCriteria");	
    testNumber = CgiStressTest::StressTest(testNumber, 3, 10, 100, 5000, &CgiStressTest::MixedCriteria, "MixedCriteria");	
    testNumber = CgiStressTest::StressTest(testNumber, 5, 50, 100, 5000, &CgiStressTest::AllValidCriteria, "AllValidCriteria");	

    close(STDERR_FILENO);

    std::cout << "\n*******************************************************************" << std::endl;
	return (0);
}