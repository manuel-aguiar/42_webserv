

// C++ headers
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <set>
#include <map>
// Project headers
#include "../HeapSlab.hpp"

#include "../../SlabAllocator/SlabAllocator.hpp"
#include "../../../_Tests/ToolkitDummy.hpp"
#include "../../../_Tests/ToolkitBase.hpp"
#include "../../../_Tests/ToolkitDerived.hpp"

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);

int main(void)
{
    int testNumber = 1;
    std::cout << "\n*************** HeapSlab tests ***************" << std::endl;

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
	
    std::cout << "*********************************************\n" << std::endl;
}


