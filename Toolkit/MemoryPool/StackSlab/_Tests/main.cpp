

// C++ headers
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <set>
#include <map>
// Project headers
#include "../StackSlab.hpp"

#include "../../SlabAllocator/SlabAllocator.hpp"
#include "../../../_Tests/ToolkitDummy.hpp"
#include "../../../_Tests/ToolkitBase.hpp"
#include "../../../_Tests/ToolkitDerived.hpp"

extern int TestPart1(int testNumber);
extern int StressTest(int testNumber);

int main(void)
{
    int testNumber = 1;
    std::cout << "\n*************** Stack Slab tests ***************" << std::endl;

    std::cout << "TEST " << testNumber++ << ": ";
    try
    {
        const size_t nodeSetSize = 40;
        const size_t arraySize = 100;

        // Initialize the StackSlabs for both the set and the map
        StackSlab<nodeSetSize, arraySize> setNodes;

        // Create SlabAllocator objects with the respective memory pools
        SlabAllocator<int, StackSlab<nodeSetSize, arraySize> > allocSet(setNodes);

        // Define a set using SlabAllocator
        typedef std::set<int, std::less<int>, SlabAllocator<int, StackSlab<nodeSetSize, arraySize> > > nodeSet;
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

        // Initialize the StackSlabs for both the set and the map
        StackSlab<nodeSetSize, arraySize> setNodes;

        // Create SlabAllocator objects with the respective memory pools
        SlabAllocator<std::pair<int, int>, StackSlab<nodeSetSize, arraySize> > allocSet(setNodes);

        // Define a set using SlabAllocator
        typedef std::map<int, int, std::less<int>, SlabAllocator<std::pair<int, int>, StackSlab<nodeSetSize, arraySize> > > nodeSet;
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
        const size_t arraySize = 100;
        const size_t nodeSetSize = 40;
        const size_t nodeMapSize = sizeof(std::pair<int, std::set<int, std::less<int>, SlabAllocator<int, StackSlab<nodeSetSize, arraySize> > > >)
									+ 4 * sizeof(void*); 

        // Initialize the StackSlabs for both the set and the map
        StackSlab<nodeSetSize, arraySize> setNodes;
        StackSlab<nodeMapSize, arraySize> mapNodes;

        // Create SlabAllocator objects with the respective memory pools
        SlabAllocator<int, StackSlab<nodeSetSize, arraySize> > allocSet(setNodes);

        // Define a set using SlabAllocator
        typedef std::set<int, std::less<int>, SlabAllocator<int, StackSlab<nodeSetSize, arraySize> > > 	nodeSet;
		typedef std::pair<const int, nodeSet>												timerPair;
		typedef SlabAllocator<timerPair, StackSlab<nodeMapSize, arraySize> >							timerAlloc;	
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


