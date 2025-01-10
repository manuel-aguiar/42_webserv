/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerTracker.tpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:17:18 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 18:24:43 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef TIMERTRACKER_TPP

# define TIMERTRACKER_TPP

// Project Headers
# include "../../../Toolkit/MemoryPool/HeapSlab/HeapSlab.hpp"
# include "../../../Toolkit/MemoryPool/SlabAllocator/SlabAllocator.hpp"

// C++ headers
# include <set>
# include <map>

template <typename T, typename U>
class TimerTracker
{
	public:
		static const size_t nodeSetSize = sizeof(U) + sizeof(void*) * 4;
		static const size_t nodeMapSize = sizeof(std::pair<const T, std::set<U> >) + sizeof(void*) * 4;

		typedef SlabAllocator<U, HeapSlab<nodeSetSize> > setAlloc;
		typedef std::set<U, std::less<U>, setAlloc > nodeSet;
		typedef std::pair<const T, nodeSet> timerPair;
		typedef SlabAllocator<timerPair, HeapSlab<nodeMapSize> > mapAlloc;
		typedef std::map<T, nodeSet, std::less<T>, mapAlloc> timerMap;

		TimerTracker(size_t capacity): 
			m_setSlab(capacity),
			m_mapSlab(capacity),
			m_capacity(capacity), 
			m_size(0), 
			m_timers(std::less<T>(), mapAlloc(m_mapSlab)), 
			m_setNodeAlloc(m_setSlab)
		{
		}

        TimerTracker(const TimerTracker& copy) :
            m_setSlab(copy.m_capacity),
            m_mapSlab(copy.m_capacity),
            m_capacity(copy.m_capacity),
            m_size(copy.m_size),
            m_timers(std::less<T>(), mapAlloc(m_mapSlab)),
            m_setNodeAlloc(m_setSlab)
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
		HeapSlab<nodeSetSize> 	m_setSlab;
		HeapSlab<nodeMapSize> 	m_mapSlab;
		size_t					m_capacity;
		size_t 					m_size;
		timerMap 				m_timers;
		setAlloc 				m_setNodeAlloc;
};

#endif