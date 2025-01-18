/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_StressTest.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:46:00 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/18 10:23:16 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <unistd.h>
# include "CgiStressTest.hpp"

int main(void)
{
	int testNumber = 1;

	std::cout << "\n*************** CgiModule Stress Tests ***************" << std::endl;
	
	testNumber = CgiStressTest::StressTest(testNumber, 5, 5, 5, 50, &CgiStressTest::ValidPython, "ValidPython");
	testNumber = CgiStressTest::StressTest(testNumber, 20, 50, 500, 150, &CgiStressTest::AllValidCriteria, "AllValidCriteria");
	//testNumber = CgiStressTest::StressTest(testNumber, 10, 100, 100, 15000, &CgiStressTest::InvalidInterpreterPath, "InvalidInterpreterPath");
	//testNumber = CgiStressTest::StressTest(testNumber, 10, 100, 100, 15000, &CgiStressTest::AllInvalidCriteria, "AllInvalidCriteria");
	//testNumber = CgiStressTest::StressTest(testNumber, 10, 500, 5000, 15000, &CgiStressTest::AllInvalidCriteria, "AllInvalidCriteria");
	//testNumber = CgiStressTest::StressTest(testNumber, 5, 100, 1000, 15000, &CgiStressTest::AllValidCriteria, "AllValidCriteria");
	//testNumber = CgiStressTest::StressTest(testNumber, 20, 500, 5000, 500, &CgiStressTest::AllValidCriteria, "AllValidCriteria");
	//testNumber = CgiStressTest::StressTest(testNumber, 5, 20, 20, 5000,   	&CgiStressTest::InvalidScript, "InvalidScript");
	//testNumber = CgiStressTest::StressTest(testNumber, 10, 100, 100, 15000, &CgiStressTest::InvalidInterpreterPath, "InvalidInterpreterPath");
	//testNumber = CgiStressTest::StressTest(testNumber, 10, 100, 100, 15000, &CgiStressTest::AllInvalidCriteria, "AllInvalidCriteria");
	//testNumber = CgiStressTest::StressTest(testNumber, 10, 100, 100, 15000, &CgiStressTest::AllInvalidCriteria, "AllInvalidCriteria");
	//testNumber = CgiStressTest::StressTest(testNumber, 5, 100, 1000, 15000, &CgiStressTest::AllValidCriteria, "AllValidCriteria");
	//testNumber = CgiStressTest::StressTest(testNumber, 5, 100, 1000, 15000, &CgiStressTest::MixedCriteria, "MixedCriteria");
	//testNumber = CgiStressTest::StressTest(testNumber, 5, 100, 1000, 15000, &CgiStressTest::InvalidInterpreter, "InvalidInterpreter");
	//testNumber = CgiStressTest::StressTest(testNumber, 5, 100, 1000, 15000, &CgiStressTest::InvalidScript, "InvalidScript");
	//testNumber = CgiStressTest::StressTest(testNumber, 5, 100, 1000, 15000, &CgiStressTest::InvalidInterpreterPath, "InvalidInterpreterPath");
	//testNumber = CgiStressTest::StressTest(testNumber, 5, 100, 1000, 15000, &CgiStressTest::AllInvalidCriteria, "AllInvalidCriteria");
	//testNumber = CgiStressTest::StressTest(testNumber, 20, 500, 5000, 15000, &CgiStressTest::AllValidCriteria, "AllValidCriteria");
	//testNumber = CgiStressTest::StressTest(testNumber, 20, 500, 5000, 15000, &CgiStressTest::AllValidCriteria, "AllValidCriteria");
	//testNumber = CgiStressTest::StressTest(testNumber, 20, 500, 5000, 80, &CgiStressTest::AllValidCriteria, "AllValidCriteria");
	//testNumber = CgiStressTest::StressTest(testNumber, 10, 500, 5000, 50, &CgiStressTest::AllValidCriteria, "AllValidCriteria");
	//testNumber = CgiStressTest::StressTest(testNumber, 10, 500, 5000, 100, &CgiStressTest::AllValidCriteria, "AllValidCriteria");
	//testNumber = CgiStressTest::StressTest(testNumber, 10, 500, 5000, 10, &CgiStressTest::AllValidCriteria, "AllValidCriteria");
	std::cout << "\n*************** ********************************" << std::endl;

	close(STDERR_FILENO);

	return (0);
}