

#include <string>
#include <iostream>
#include <fstream>
#include "../../../../Toolkit/TestHelpers/TestHelpers.h"


extern bool DirectoryListing(const std::string& path, std::string& output);

int main(void)
{
	int testNumber = 1;
	TEST_HEADER("Directory Listing");

	try
	{
		TEST_INTRO(testNumber++);

		std::string path = "asgasgasg";
		std::string output;

		EXPECT_EQUAL(DirectoryListing(path, output), false, "directory listing should not be successfull");

		TEST_PASSED_MSG("bogus path");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
//////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);

		std::string path = "/doesnt/exist/";
		std::string output;

		EXPECT_EQUAL(DirectoryListing(path, output), false, "directory listing should not be successfull");

		TEST_PASSED_MSG("path that doesnt exist");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

////////////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);

		std::string path = "main.cpp";
		std::string output;

		EXPECT_EQUAL(DirectoryListing(path, output), false, "directory listing should not be successfull");

		TEST_PASSED_MSG("path that exists but is not a folder");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	////////////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);

		std::string path = "../../";
		std::string output;

		EXPECT_EQUAL(DirectoryListing(path, output), true, "directory listing should be successfull");
		
		std::ofstream file("directoryListing.html");
		if (file.is_open())
		{
			file << output;
			file.close();
		}

		TEST_PASSED_MSG("valid folder path");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}


	TEST_FOOTER;

    return (0);
}