/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test3.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:46:00 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 11:43:10 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

extern int Impl_StressTest(int testNumber, const int workers, const int backlog, const int connectionCount, const unsigned int timeoutMs);

int TestPart3(int testNumber)
{

	//testNumber = Impl_StressTest(testNumber, 3, 3, 3);
	//testNumber = Impl_StressTest(testNumber, 10, 100, 100);
	//testNumber = Impl_StressTest(testNumber, 1, 54, 55);					
	testNumber = Impl_StressTest(testNumber, 50, 200, 1000, 5000);
	//testNumber = Impl_StressTest(testNumber, 50, 300, 2000);
	
    return (testNumber);
}
// MINIMO QUE REPRODUZ, ATE AGORA
//(testNumber, 1, 54, 55)