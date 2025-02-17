
// Project headers
# include "../../BufferView/BufferView.hpp"
# include "../../../../Toolkit/TestHelpers/TestHelpers.h"


// C++ headers
# include <iostream>


int main(void)
{
	int testNumber = 1;
	TEST_HEADER("BufferView");

	try
	{
		TEST_INTRO(testNumber++);

		std::string str = "Hello World!";

		// creating a BufferView out of a std::string
		BufferView sv(str.c_str(), str.size());
		
		// confirm sizes
		EXPECT_EQUAL(sv.size(), str.size(), "size should be equal to string size");

		// looking for byte strings inside the view, NOTICE THE SEARCH IS NOT INCLUDING THE NULL BYTE
		EXPECT_EQUAL(sv.find("Hello", 5), 0, "should find world at index 0");
		EXPECT_EQUAL(sv.find("World", 5), 6, "should find world at index 6");

		// looking and including the NULL byte in the search, will fail and return npos
		EXPECT_EQUAL(sv.find("Hello", 6), BufferView::npos, "search includes the null byte, won't match");
		
		// substr test
		BufferView sub = sv.substr(6, 5); // starting at index 6, 5 characters

		EXPECT_EQUAL(sub.find("World", 5), 0, "should find world at index 0 int BufferView substr");

		std::string assigned;
		
		// generating a null terminated string out of BufferView
		sub.to_string(assigned);

		EXPECT_EQUAL(assigned, "World", "should be equal to World");

		TEST_PASSED_MSG("simple test");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
	



	TEST_FOOTER;
}
