
// Project headers
# include "../../BufferView/BufferView.hpp"
# include "../../../../Toolkit/TestHelpers/TestHelpers.h"


// C++ headers
# include <iostream>

static const char* headersOfInterest[] = 
{
	"Accept",
	"Accept-Encoding",
	"Accept-Language",
	"Connection",
	"Content-Length",
	"Content-Type",
	"Host",
	"Proxy-Connection",
	"Transfer-Encoding",
};

// binary search into headersOfInterest to see if we find our target
static int binSearch(const char** lookup, size_t sizeOfLookup, const BufferView& target)
{
	int        		low = 0;
	int        		high = sizeOfLookup - 1;
	int        		mid;
	BufferView 		midView;

	if (sizeOfLookup <= 0)
		return (-1);

	while (low < high)
	{
		mid = low + ((high - low) / 2);
		midView = BufferView(lookup[mid]);
		if (midView == target)
			return (mid);
		else if (midView > target)
			high = mid - 1;
		else
			low = mid + 1;
	}

	if (target != BufferView(lookup[low]))
		return (-1);

	return (low);
}


int main(void)
{
	int testNumber = 1;
	TEST_HEADER("BufferView");

	try
	{
		TEST_INTRO(testNumber++);

		EXPECT_EQUAL(binSearch(headersOfInterest, sizeof(headersOfInterest) / sizeof(headersOfInterest[0]), BufferView("Accept-Encoding")) != -1, true, 
		"should find Accept");

		TEST_PASSED_MSG("simple test");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

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
		assigned = sub.to_string();
		
		EXPECT_EQUAL(BufferView("Accept-Encoding") > BufferView("Accept"), true, "should be greater than");
		EXPECT_EQUAL(BufferView("Accept-Encoding") >= BufferView("Accept"), true, "should be greater than");
		EXPECT_EQUAL(BufferView("Accept") < BufferView("Accept-Encoding"), true, "should be greater than");
		EXPECT_EQUAL(BufferView("Accept") <= BufferView("Accept-Encoding"), true, "should be greater than");
		EXPECT_EQUAL(BufferView("Accept") == BufferView("Accept"), true, "should be greater than");
		EXPECT_EQUAL(BufferView("Accept") != BufferView("Accept-Encoding"), true, "should be greater than");

		EXPECT_EQUAL(assigned, "World", "should be equal to World");

		BufferView test("   Hello World!   ");
		BufferView trimmed = test.trim(BufferView(" "));

		EXPECT_EQUAL(trimmed, "Hello World!", "should be equal to Hello World!");
		TEST_PASSED_MSG("simple test");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	try
	{
		TEST_INTRO(testNumber++);

		std::string str = "\t   \r hello world!  \v  \n";
		
		BufferView trimmed = BufferView(str).trim(BufferView(" \t\r\n\v"));
		EXPECT_EQUAL(trimmed, BufferView("hello world!"), "should be equal to Hello World!");

		trimmed.modify_ToCapitalized();
		EXPECT_EQUAL(trimmed, BufferView("Hello World!"), "should be equal to Hello World!");

		std::string contentType = "content-type: text/html";
		BufferView capitalized = BufferView(contentType).modify_ToCapitalized();
		EXPECT_EQUAL(capitalized, "Content-Type: Text/Html", "should be equal to Content-Type: Text/Html");

		capitalized.modify_ToUpperCase();

		EXPECT_EQUAL(capitalized, "CONTENT-TYPE: TEXT/HTML", "should be equal to CONTENT-TYPE: TEXT/HTML");

		capitalized.modify_ToCapitalized();

		EXPECT_EQUAL(capitalized, "Content-Type: Text/Html", "should be equal to Content-Type: Text/Html");
		

		BufferView chained = BufferView(str).trim(BufferView(" \t\r\n\v")).modify_ToCapitalized();
		EXPECT_EQUAL(chained, "Hello World!", "should be equal to Hello World!");

		TEST_PASSED_MSG("trimming, modification tests");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
	
	




	TEST_FOOTER;
}
