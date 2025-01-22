


// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

// Project headers
# include "../HeapArray.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../_Tests/TestHelpers.h"


int TestPart3(int testNumber)
{
    try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::vector<ToolkitBase*> 		std;
		HeapArray<ToolkitBase*> 		array(500);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(new ToolkitBase(i));
			array.push_back(new ToolkitBase(i));

			std.push_back(new ToolkitDerived(i));
			array.push_back(new ToolkitDerived(i));

			std.push_back(new ToolkitDerived(i));
			array.push_back(new ToolkitDerived(i));

			delete std.back();
			delete array.back();

			std.pop_back();
			array.pop_back();
			std.push_back(new ToolkitBase(i));
			array.push_back(new ToolkitBase(i));
		}
		if (std.size() != array.size())
			throw std::runtime_error("size mismatch, got: " + TestHelpers::to_string(array.size()) + " expected: " + TestHelpers::to_string(std.size()) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		HeapArray<ToolkitBase*>::iterator it = array.begin();
		std::vector<ToolkitBase*>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (**it != **iter)
				throw std::runtime_error("value mismatch " + '\n'
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

		it = array.begin();
		iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			delete (*it);
			delete (*iter);
		}


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    return (testNumber);
}