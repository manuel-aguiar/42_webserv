/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 13:55:45 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/27 15:01:40 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <climits>
# include <cstddef>
# include <exception>
# include <stdexcept>
# include <iostream>
# include <cassert>
# include "../Nginx_MemoryPool.hpp"
# include <cstring>

# include "../Nginx_MemoryPool.hpp"
# include "../Nginx_PoolAllocator.hpp"



// inherits, that's the one we will use to count
# include "NginxAllocCounters.tpp"

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);

int main(void)
{
    int testNumber = 1;

	std::cout << "\n*************** Nginx_MemoryPool tests ***************" << std::endl;
	
    testNumber = TestPart1(testNumber);
	testNumber = TestPart2(testNumber);

	std::cout << "******************************************************\n" << std::endl;

    return 0;
}