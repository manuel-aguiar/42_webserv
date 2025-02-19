// target
# include "../../Buffer/Buffer.hpp"

// project headers
#include "../../../../Toolkit/TestHelpers/TestHelpers.h"

// c++ headers
#include <iostream>

// c headers
# include <unistd.h>
# include <fcntl.h>
# include <cstring> // strlen

int main(void)
{
	int testNumber = 1;

	TEST_HEADER("Buffer");

	try
	{
		TEST_INTRO(testNumber++);
		
		// open file, read into buffer, close file
		int fd = ::open("main.cpp", O_RDONLY, 777);
		if (fd == -1)
			throw std::runtime_error("failed to open file");
		Buffer<1024> buffer;
		buffer.read(fd);
		::close(fd);

		// get a view of the buffer, to do searches and substrings
		BufferView view = buffer.view();
		
		// should be at position 0
		EXPECT_EQUAL(view.find("int main(void)", std::strlen("int main(void)")) != BufferView::npos, true, "should find the main function");

		TEST_PASSED_MSG("simple test");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

/////////////////////////////////////////////////////////

try
{
	TEST_INTRO(testNumber++);
	
	// open file, read into buffer, close file
	int fd = ::open("testInput.txt", O_RDONLY, 777);
	if (fd == -1)
		throw std::runtime_error("failed to open file");
	Buffer<1024> buffer;
	buffer.read(fd);
	::close(fd);

	buffer.clear();
	// get a view of the buffer, to do searches and substrings
	BufferView view = buffer.view();
	
	EXPECT_EQUAL(view.size(), 0, "should be empty");
	EXPECT_EQUAL(view.find("int main(void)", std::strlen("int main(void)")), BufferView::npos, "should not find anything, it is empty");

	TEST_PASSED_MSG("testing clear");
}
catch(const std::exception& e)
{
	TEST_FAILED_MSG(e.what());
}

///////////////////////////////////////////////////	
	try
	{
		TEST_INTRO(testNumber++);
		
		std::string str = "Hello World!";
		// open file to write
		int fd = ::open("testOutput.txt", O_RDWR | O_TRUNC | O_CREAT, 777);

		if (fd == -1)
			throw std::runtime_error("failed to open file");

		Buffer<1024> buffer;
		
		// pushing, both const char* and a std::string
		buffer.push(str.c_str(), str.size());
		buffer.push(str);

		BufferView view = buffer.view();
		EXPECT_EQUAL(view.size(), str.size() + str.size(), "should be equal to Hello World!Hello World!");

		buffer.write(fd);

		::close(fd);

		// open the file again for reading, read into the same buffer, close file
		fd = ::open("testOutput.txt", O_RDONLY, 777);
		buffer.read(fd);
		::close(fd);


		std::string checkInternal;

		// using BufferView to get a string out of the buffer -> places a null byte at the end. NO
		// GUARANTEE THAT THERE ARE NO NULL BYTES BEFORE WHERE YOU "EXPECT" THEM TO BE
		buffer.view().to_string(checkInternal);
		EXPECT_EQUAL(checkInternal, str + str, "should be equal to Hello World!Hello World!");

		TEST_PASSED_MSG("test push and read");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}


	///////////////////////////////////////////////////	
	try
	{
		TEST_INTRO(testNumber++);
		
		std::string str = "Hello World!";
		// open file to write
		int fd = ::open("testOutput.txt", O_RDWR | O_TRUNC | O_CREAT, 777);
		Buffer<1024> buffer;
		
		// push and write to fd, using PUSH OF A CONST CHAR*
		buffer.push(str.c_str(), str.size());
		
		buffer.write(fd);
		::close(fd);

		// read the first time, close and reopen to restart offsets
		fd = ::open("testOutput.txt", O_RDONLY, 777);
		if (fd == -1)
			throw std::runtime_error("failed to open file");
		buffer.read(fd);
		::close(fd);

		// read the first time, close and reopen to restart offsets
		fd = ::open("testOutput.txt", O_RDONLY, 777);
		if (fd == -1)
			throw std::runtime_error("failed to open file");
		buffer.read(fd, buffer.size());						// start reading where you left off before
		::close(fd);


		std::string checkInternal;

		// using BufferView to get a string out of the buffer
		buffer.view().to_string(checkInternal);
		EXPECT_EQUAL(checkInternal, str + str, "should be equal to Hello World!Hello World!");

		TEST_PASSED_MSG("multiple offset reads");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	TEST_FOOTER;
}