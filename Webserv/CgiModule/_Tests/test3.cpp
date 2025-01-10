/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test3.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:46:00 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 10:41:08 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

extern int Impl_StressTest(int testNumber, const int workers, const int backlog, const int connectionCount);

int TestPart3(int testNumber)
{

	testNumber = Impl_StressTest(testNumber, 1, 1, 1);
	testNumber = Impl_StressTest(testNumber, 10, 100, 100);
	testNumber = Impl_StressTest(testNumber, 5, 100, 1000);
	testNumber = Impl_StressTest(testNumber, 50, 200, 1000);
	testNumber = Impl_StressTest(testNumber, 50, 500, 5000);
	
    return (testNumber);
}