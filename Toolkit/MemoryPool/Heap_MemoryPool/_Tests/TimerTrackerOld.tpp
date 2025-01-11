/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerTrackerOld.tpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:17:18 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 20:05:56 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef TIMERTRACKEROLD_TPP

# define TIMERTRACKEROLD_TPP

// Project Headers
# include "../Heap_MemoryPool.hpp"
# include "../../Nginx_PoolAllocator/Nginx_PoolAllocator.hpp"
# include "../../HeapSlab/HeapSlab.hpp"
# include "../../SlabAllocator/SlabAllocator.hpp"

// C++ headers
# include <set>
# include <map>
# include <memory>



template <typename T, typename U, typename Allocator>
class TimerTrackerOld
{
	public:

		template <size_t OutterSize>
		struct AlignedStorage
		{
			template <size_t InnerSize, size_t Alignment>
			struct AlignedSize
			{
				static const size_t value = (InnerSize + Alignment - 1) & ~(Alignment - 1);
			};
			static const size_t value = AlignedSize<OutterSize, __alignof__(OutterSize)>::value;
		};

		static const size_t nodeSetSize = AlignedStorage<sizeof(U) + sizeof(void*) * 3>::value;
		static const size_t nodeMapSize = AlignedStorage<sizeof(std::pair<const T, std::set<U> >) + sizeof(void*) * 3>::value;

		typedef Heap_MemoryPool<typename Allocator::template rebind<unsigned char>::other>
																			fixedBlock;
		typedef Nginx_PoolAllocator<T, fixedBlock> 							poolAlloc;
		typedef HeapSlab<nodeSetSize, poolAlloc>							setSlab;
		typedef HeapSlab<nodeMapSize, poolAlloc>							mapSlab;
		typedef SlabAllocator<U, setSlab> 									setAlloc;
		typedef std::set<U, std::less<U>, setAlloc > 						nodeSet;
		typedef std::pair<const T, nodeSet> 								timerPair;
		typedef SlabAllocator<timerPair, mapSlab > 							mapAlloc;
		typedef std::map<T, nodeSet, std::less<T>, mapAlloc> 				timerMap;

		TimerTrackerOld(size_t capacity, const Allocator& allocator = Allocator()) : 
			m_fixedBlock(nodeSetSize * capacity + nodeMapSize * capacity, allocator),		//check this, double capacity for nothing
			m_setSlab(capacity, poolAlloc(m_fixedBlock)),
			m_mapSlab(capacity, poolAlloc(m_fixedBlock)),
			m_capacity(capacity), 
			m_size(0), 
			m_timers(std::less<T>(), mapAlloc(m_mapSlab)), 
			m_setNodeAlloc(m_setSlab)
		{
		}

        TimerTrackerOld(const TimerTrackerOld& copy) :
            m_setSlab(copy.m_capacity),
            m_mapSlab(copy.m_capacity),
            m_capacity(copy.m_capacity),
            m_size(copy.m_size),
            m_timers(std::less<T>(), mapAlloc(m_mapSlab)),
            m_setNodeAlloc(m_setSlab)
        {
            *this = copy;
        }
        
        TimerTrackerOld& operator=(const TimerTrackerOld& copy)
        {
            if (this == &copy)
                return (*this);

            m_capacity = copy.m_capacity;
            m_size = copy.m_size;
            m_timers = copy.m_timers;

            return (*this);
        }

		void insert(T key, T value)
		{
			typename timerMap::iterator it = m_timers.find(key);

			if (it == m_timers.end())
			{
				nodeSet set1(std::less<T>(), m_setNodeAlloc);
				set1.insert(value);
				m_timers.insert(std::pair<T, nodeSet>(key, set1));
			}
			else
			{
				it->second.insert(value);
			}
		}

		void erase(T key, T value)
		{
			typename timerMap::iterator it = m_timers.find(key);

			if (it != m_timers.end())
			{
				it->second.erase(value);
			}
		}

		void erase(T key)
		{
			typename timerMap::iterator it = m_timers.find(key);

			if (it != m_timers.end())
			{
				m_timers.erase(it);
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
				std::cout << it->first << ": ";
				for (typename nodeSet::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
				{
					std::cout << *it2 << " ";
				}
				std::cout << std::endl;
			}
		}

	private:
		fixedBlock 							m_fixedBlock;
		setSlab							 	m_setSlab;
		mapSlab							 	m_mapSlab;
		size_t								m_capacity;
		size_t 								m_size;
		timerMap 							m_timers;
		setAlloc 							m_setNodeAlloc;
};

#endif