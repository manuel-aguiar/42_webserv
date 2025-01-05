/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 13:55:45 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/03 17:31:11 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include <exception>
# include <stdexcept>
# include <iostream>

# include "../Nginx_MemoryPool.hpp"
# include "../Nginx_PoolAllocator.hpp"


extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);
extern int TestPart3(int testNumber);

int main(void)
{
    int testNumber = 1;

	std::cout << "\n*************** Nginx_MemoryPool tests ***************" << std::endl;
	
    testNumber = TestPart1(testNumber);
	testNumber = TestPart2(testNumber);
    testNumber = TestPart3(testNumber);

	std::cout << "******************************************************\n" << std::endl;

    return 0;
}