/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_StressTest.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:46:00 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/12 00:06:43 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include "CgiStressTest.hpp"

int main(void)
{
	int testNumber = 1;

	std::cout << "\n*************** CgiModule Stress Tests ***************" << std::endl;

	testNumber = CgiStressTest::StressTest(testNumber, 1, 1, 1, 5000,   	&CgiStressTest::MixedCriteria, "MixedCriteria");
	testNumber = CgiStressTest::StressTest(testNumber, 10, 100, 100, 5000, &CgiStressTest::MixedCriteria, "MixedCriteria");
	testNumber = CgiStressTest::StressTest(testNumber, 10, 100, 100, 5000, &CgiStressTest::AllInvalidCriteria, "AllInvalidCriteria");
	testNumber = CgiStressTest::StressTest(testNumber, 10, 100, 100, 5000, &CgiStressTest::AllInvalidCriteria, "AllInvalidCriteria");
	testNumber = CgiStressTest::StressTest(testNumber, 5, 100, 1000, 5000, &CgiStressTest::MixedCriteria, "MixedCriteria");

	testNumber = CgiStressTest::StressTest(testNumber, 50, 500, 5000, 200, &CgiStressTest::AllSuccessCriteria, "AllSuccessCriteria");
	testNumber = CgiStressTest::StressTest(testNumber, 50, 500, 5000, 100, &CgiStressTest::AllSuccessCriteria, "AllSuccessCriteria");
	testNumber = CgiStressTest::StressTest(testNumber, 20, 500, 5000, 100, &CgiStressTest::AllSuccessCriteria, "AllSuccessCriteria");
	testNumber = CgiStressTest::StressTest(testNumber, 20, 500, 5000, 80, &CgiStressTest::AllSuccessCriteria, "AllSuccessCriteria");
	testNumber = CgiStressTest::StressTest(testNumber, 10, 500, 5000, 50, &CgiStressTest::AllSuccessCriteria, "AllSuccessCriteria");
	testNumber = CgiStressTest::StressTest(testNumber, 10, 500, 5000, 100, &CgiStressTest::AllSuccessCriteria, "AllSuccessCriteria");
	testNumber = CgiStressTest::StressTest(testNumber, 10, 500, 5000, 10, &CgiStressTest::AllSuccessCriteria, "AllSuccessCriteria");
	std::cout << "\n*************** ********************************" << std::endl;

	return (0);
}