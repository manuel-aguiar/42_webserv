/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test3.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:46:00 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 13:11:25 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

extern int Impl_StressTest(int testNumber, const int workers, const int backlog, const int connectionCount, const unsigned int timeoutMs);
extern int Impl_StallTest(int testNumber, const int workers, const int backlog, const int connectionCount, const unsigned int timeoutMs);



int TestPart3(int testNumber)
{

	testNumber = Impl_StressTest(testNumber, 3, 3, 3, 5000);
	testNumber = Impl_StressTest(testNumber, 10, 100, 100, 5000);
	testNumber = Impl_StressTest(testNumber, 1, 54, 55, 5000);					
	testNumber = Impl_StressTest(testNumber, 50, 50, 500, 5000);
	testNumber = Impl_StallTest(testNumber, 10, 10, 500, 5000);
	
    return (testNumber);
}
// MINIMO QUE REPRODUZ, ATE AGORA
//(testNumber, 1, 54, 55)