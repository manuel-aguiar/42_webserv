/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 08:40:58 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 18:20:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// C++ headers
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <set>
#include <map>
// Project headers
#include "../HeapSlab.tpp"
#include "../../SlabAllocator/SlabAllocator.hpp"
#include "../../../_Tests/ToolkitDummy.hpp"
#include "../../../_Tests/ToolkitBase.hpp"
#include "../../../_Tests/ToolkitDerived.hpp"

extern int TestPart1(int testNumber);
extern int StressTest(int testNumber);

template <typename T, typename U>
class TimerTracker {
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

int main(void)
{
    int testNumber = 1;
    std::cout << "\n*************** HeapSlotArray tests ***************" << std::endl;

    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        const size_t nodeSetSize = 40;
        const size_t arraySize = 100;

        // Initialize the HeapSlabs for both the set and the map
        HeapSlab<nodeSetSize> setNodes(arraySize);

        // Create SlabAllocator objects with the respective memory pools
        SlabAllocator<int, HeapSlab<nodeSetSize> > allocSet(setNodes);

        // Define a set using SlabAllocator
        typedef std::set<int, std::less<int>, SlabAllocator<int, HeapSlab<nodeSetSize> > > nodeSet;
		nodeSet set1(std::less<int>(), allocSet);

		set1.insert(1);

        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "	FAILED: " << e.what() << std::endl;
    }

    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        const size_t nodeSetSize = 48;
        const size_t arraySize = 100;

        // Initialize the HeapSlabs for both the set and the map
        HeapSlab<nodeSetSize> setNodes(arraySize);

        // Create SlabAllocator objects with the respective memory pools
        SlabAllocator<std::pair<int, int>, HeapSlab<nodeSetSize> > allocSet(setNodes);

        // Define a set using SlabAllocator
        typedef std::map<int, int, std::less<int>, SlabAllocator<std::pair<int, int>, HeapSlab<nodeSetSize> > > nodeSet;
		nodeSet map1(std::less<int>(), allocSet);

		map1[0] = 1;

        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "	FAILED: " << e.what() << std::endl;
    }

    std::cout << "TEST " << testNumber++ << ": ";

 	try
    {
        const size_t nodeSetSize = 40;
        const size_t nodeMapSize = sizeof(std::pair<int, std::set<int, std::less<int>, SlabAllocator<int, HeapSlab<nodeSetSize> > > >)
									+ 4 * sizeof(void*); 
        const size_t arraySize = 100;

        // Initialize the HeapSlabs for both the set and the map
        HeapSlab<nodeSetSize> setNodes(arraySize);
        HeapSlab<nodeMapSize> mapNodes(arraySize);

        // Create SlabAllocator objects with the respective memory pools
        SlabAllocator<int, HeapSlab<nodeSetSize> > allocSet(setNodes);

        // Define a set using SlabAllocator
        typedef std::set<int, std::less<int>, SlabAllocator<int, HeapSlab<nodeSetSize> > > 	nodeSet;
		typedef std::pair<const int, nodeSet>												timerPair;
		typedef SlabAllocator<timerPair, HeapSlab<nodeMapSize> >							timerAlloc;	
		typedef std::map<int, nodeSet, std::less<int>, timerAlloc>							timerMap;

		timerAlloc allocMap(mapNodes);
        timerMap map1(std::less<int>(), allocMap);

        // Initialize and assign a set to the map
        nodeSet set1(std::less<int>(), allocSet);
        map1.insert(std::pair<int, nodeSet>(1, set1));

		map1.begin()->second.insert(1);

        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "	FAILED: " << e.what() << std::endl;
    }

	try
	{
		TimerTracker<int, int> timers(100);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
    std::cout << "*********************************************\n" << std::endl;
}


