/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NginxAllocCounters.tpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 09:01:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/27 14:03:17 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef NGINXALLOCCOUNTERS_TPP

# define NGINXALLOCCOUNTERS_TPP

# include "../Nginx_MemoryPool.hpp"

class NginxAllocCounter  : public Nginx_MemoryPool
{
	public:
	
		enum counterType
		{
			ALLOC = 0,
			DEALLOC,
			COUNT,
		};

		static NginxAllocCounter* create(size_t blockSize, size_t* placeCounters, size_t* placeTotalBytes, size_t startingBlocks = 1)
		{
			// First, allocate raw memory for NginxAllocCounter
			Nginx_MemoryPool* pool = Nginx_MemoryPool::create(blockSize, startingBlocks);
			
			// Use placement new to construct the NginxAllocCounter object in the allocated memory
			NginxAllocCounter* counter = (NginxAllocCounter*)pool->allocate(sizeof(NginxAllocCounter));
			
			counter->m_pool = pool;
			counter->m_placeCounters = placeCounters;
			counter->m_placeTotalBytes = placeTotalBytes;
			return counter;
		}

		// Override allocate function to count allocations
		void* allocate(size_t size)  {
			void* ptr = m_pool->allocate(size);
			m_placeCounters[ALLOC]++;  // Increment allocation count
			(*m_placeTotalBytes) += size;
			return ptr;
		}

		void* allocate(size_t size, size_t alignment)  {
			void* ptr = m_pool->allocate(size, alignment);
			m_placeCounters[DEALLOC]++;  // Increment allocation count
			(*m_placeTotalBytes) += size;
			return ptr;
		}

		// Reset the counter or memory pool as needed
		void reset(int maxBlocks = INT_MAX)  {
			m_pool->reset(maxBlocks);  // Call base class reset
		}

		// Destroy the pool
		void destroy()  {
			m_pool->destroy();  // Call base class destroy
			// Additional cleanup if needed
		}

		Nginx_MemoryPool*	m_pool;
		size_t* 			m_placeCounters;
		size_t* 			m_placeTotalBytes;

};


#endif