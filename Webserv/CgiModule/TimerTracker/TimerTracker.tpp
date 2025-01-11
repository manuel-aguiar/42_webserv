/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerTracker.tpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:17:18 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 00:18:14 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef TIMERTRACKER_TPP

# define TIMERTRACKER_TPP

// Project Headers
# include "../../../Toolkit/MemoryPool/HeapFixedBlock/HeapFixedBlock.hpp"
# include "../../../Toolkit/MemoryPool/Nginx_PoolAllocator/Nginx_PoolAllocator.hpp"
# include "../../../Toolkit/MemoryPool/HeapSlab/HeapSlab.hpp"
# include "../../../Toolkit/MemoryPool/SlabAllocator/SlabAllocator.hpp"
# include "../../../Toolkit/MemoryPool/FixedBlock_MemoryPool/FixedBlock_MemoryPool.hpp"

// C++ headers
# include <map>

/*
	After refactoring, it is just a multimap, sad :(
*/

template <typename T, typename U, typename Allocator>
class TimerTracker
{
	public:

		typedef std::pair<const T, U> 										timerPair;
		typedef FixedBlock_MemoryPool<timerPair, typename Allocator::template rebind<timerPair>::other>
		 																	mapPool;
		typedef std::multimap<T, U, std::less<T>, mapPool> 					timerMap;

		TimerTracker(size_t capacity, const Allocator& allocator = Allocator()) : 
			m_mapPool(capacity, typename Allocator::template rebind<timerPair>::other(allocator)),
			m_timers(std::less<T>(), m_mapPool), 
			m_capacity(capacity), 
			m_size(0)
		{
		}

        TimerTracker(const TimerTracker& copy) :
			m_mapPool(copy.m_capacity, typename Allocator::template rebind<timerPair>::other(copy.m_mapPool.getAllocator())),
            m_timers(std::less<T>(), m_mapPool),
            m_capacity(copy.m_capacity),
            m_size(copy.m_size)
        {
            *this = copy;
        }
        
        TimerTracker& operator=(const TimerTracker& copy)
        {
            if (this == &copy)
                return (*this);

            m_capacity = copy.m_capacity;
            m_size = copy.m_size;
            m_timers = copy.m_timers;

            return (*this);
        }

		void insert(const T& key, const U& value)
		{
			m_timers.insert(std::pair<T, U>(key, value));
		}

		void erase(const T& key, const U& value)
		{
			std::pair<typename timerMap::iterator, typename timerMap::iterator> it = m_timers.equal_range(key);
			
			//can be multiple, but not in our case
			for (typename timerMap::iterator start = it.first; start != it.second; ++start)
			{
				if (start->second == value)
				{
					m_timers.erase(start);
					break;
				}
			}
		}

		void clear()
		{
			m_timers.clear();
		}

		void print()
		{
			for (typename timerMap::iterator it = m_timers.begin(); it != m_timers.end(); ++it)
			{
				std::cout << it->first << ": " << it->second << std::endl;
			}
		}

	private:
		FixedBlock_MemoryPool<std::pair<const T, U>, typename Allocator::template rebind<std::pair<const T, U> >::other>
											m_mapPool;
		timerMap 							m_timers;
		size_t								m_capacity;
		size_t 								m_size;
};

#endif