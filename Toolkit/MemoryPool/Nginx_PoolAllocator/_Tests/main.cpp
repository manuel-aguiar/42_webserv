/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 13:55:45 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 00:50:03 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// C++ headers
# include <exception>
# include <stdexcept>
# include <iostream>

// Project
# include "../../Nginx_MemoryPool/Nginx_MemoryPool.hpp"
# include "../../Heap_MemoryPool/Heap_MemoryPool.hpp"
# include "../../Nginx_PoolAllocator/Nginx_PoolAllocator.hpp"


extern int TestPart1(int testNumber);

int main(void)
{
    int testNumber = 1;
    

	std::cout << "\n*************** Nginx_PoolAllocator tests ***************" << std::endl;
	
    testNumber = TestPart1(testNumber);

    std::cout << "******************************************************\n" << std::endl;

    return 0;
}