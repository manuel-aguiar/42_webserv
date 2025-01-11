/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerTracker.tpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:17:18 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 11:30:47 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef TIMERTRACKER_TPP

# define TIMERTRACKER_TPP

// Project Headers
# include "../../../Toolkit/MemoryPool/Heap_ObjectPool/Heap_ObjectPool.hpp"

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
		typedef typename Allocator::template rebind<timerPair>::other		pairAllocator;
		typedef Heap_ObjectPool<timerPair, pairAllocator>					mapPool;
		typedef std::multimap<T, U, std::less<T>, mapPool> 					timerMap;

		typedef typename timerMap::iterator iterator;
    	typedef typename timerMap::const_iterator const_iterator;

		TimerTracker(size_t capacity, const Allocator& allocator = Allocator()) : 
			m_timers(std::less<T>(), mapPool(capacity, pairAllocator(allocator))), 
			m_capacity(capacity), 
			m_size(0)
		{
		}

        TimerTracker(const TimerTracker& copy) :
            m_timers(std::less<T>(), mapPool(copy.m_capacity, pairAllocator(copy.m_timers.get_allocator()))),
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

		iterator insert(const T& key, const U& value)
		{
			return (m_timers.insert(std::pair<T, U>(key, value)));
		}

		void erase(const T& key, const U& value)
		{
			std::pair<iterator, iterator> it = m_timers.equal_range(key);
			
			//can be multiple, but not in our case
			for (iterator start = it.first; start != it.second; ++start)
			{
				if (start->second == value)
				{
					m_timers.erase(start);
					break;
				}
			}
		}

		void erase(iterator it)
		{
			m_timers.erase(it);
		}

		void clear()
		{
			m_timers.clear();
		}
		
		iterator begin() 				{ return (m_timers.begin()); }
		iterator end() 					{ return (m_timers.end()); }
		const_iterator begin() const 	{ return (m_timers.begin()); }
		const_iterator end() const 		{ return (m_timers.end()); }

		void print()
		{
			for (typename timerMap::iterator it = m_timers.begin(); it != m_timers.end(); ++it)
			{
				std::cout << it->first << ": " << it->second << std::endl;
			}
		}

	private:
		timerMap 							m_timers;
		size_t								m_capacity;
		size_t 								m_size;
};

#endif