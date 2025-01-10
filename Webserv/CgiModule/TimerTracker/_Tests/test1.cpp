/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 19:00:51 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 20:01:14 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "../TimerTracker.hpp"
# include "../../../../Toolkit/MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.hpp"
# include "../../../../Toolkit/MemoryPool/Nginx_PoolAllocator/Nginx_PoolAllocator.hpp"

int TestPart1(int testNumber)
{

    try
    {
        std::cout << "TEST " << testNumber++ << ": ";
        
        Nginx_MemoryPool pool(20000, 1);

        //some BS, will just rebind for internal arrays
        Nginx_PoolAllocator<int> alloc(pool);

        TimerTracker<int, int, Nginx_PoolAllocator<int> > tracker(123, alloc);



		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    return (testNumber);
}