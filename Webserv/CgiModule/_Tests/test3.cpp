/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test3.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:46:00 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 18:49:15 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CgiStressTest.hpp"


int TestPart3(int testNumber)
{

	testNumber = CgiStressTest::Impl_StressTest(testNumber, 3, 3, 3, 500, &CgiStressTest::AllSuccessCriteria);
	testNumber = CgiStressTest::Impl_StressTest(testNumber, 5, 100, 100, 1500, &CgiStressTest::MixedCriteria);	
	testNumber = CgiStressTest::Impl_StressTest(testNumber, 5, 50, 100, 2000, &CgiStressTest::MixedCriteria);		
	testNumber = CgiStressTest::Impl_StressTest(testNumber, 10, 50, 500, 2000, &CgiStressTest::MixedCriteria);			


    return (testNumber);
}
// MINIMO QUE REPRODUZ, ATE AGORA
//(testNumber, 1, 54, 55)