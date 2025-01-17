/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_FunctionalTest.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 10:57:28 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/16 12:46:15 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <unistd.h>

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);
//extern int SpecificTest(int testNumber);

int main(void)
{
	int testNumber = 1;
	std::cout << "\n*************** CgiModule Functional Tests ***************" << std::endl;

	testNumber = TestPart1(testNumber);
	testNumber = TestPart2(testNumber);
	//testNumber = SpecificTest(testNumber);

	close(STDERR_FILENO);



	std::cout << "***********************************************\n" << std::endl;
}
