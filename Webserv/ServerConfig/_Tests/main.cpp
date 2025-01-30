#include <iostream>
#include <string>
#include <stdexcept>  // For std::exception, std::runtime_error, etc.
#include <dirent.h>

#include "../ServerConfig/ServerConfig.hpp"
#include "../ServerBlock/ServerBlock.hpp"
#include "../ServerLocation/ServerLocation.hpp"
#include "../DefaultConfig/DefaultConfig.hpp"
#include "../../GenericUtils/WsTestHelpers/WsTestHelpers.h" 
#include <unistd.h>
#include <cstdio>

#define TESTFOLDER std::string("./") 
#define CLR_BLUE "\033[34m"
#define CLR_RESET "\033[0m"
#define CLR_PASS "\033[32mPass\033[0m"	
#define CLR_FAIL "\033[31mFail\033[0m"


void testFunction(const std::string &filePath, int testNbr, bool expectedValid)
{
									//remove stderr prints
									int testpipe[2];
									int stdcerrDup = dup(STDERR_FILENO);
									pipe(testpipe);
									dup2(testpipe[1], STDERR_FILENO);

	TEST_INTRO(testNbr);

	bool passed = false;

	ServerConfig config(filePath.c_str(), NULL);
	if (config.parseConfigFile() == (int)expectedValid)
		passed = true;
									//cleanup
									dup2(stdcerrDup, STDERR_FILENO);
									close(stdcerrDup);
									close(testpipe[1]);
									close(testpipe[0]);

	if (passed)
		TEST_PASSED_MSG(filePath);
	else
		TEST_FAILED_MSG(filePath);
}

void testFolder(const std::string &folderPath, int& testNumber, bool expectedValid)
{
	DIR		*dir;
	struct	dirent *ent;

	if ((dir = opendir(folderPath.c_str())) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_type == DT_REG) {
				std::string filePath = folderPath + "/" + ent->d_name;
				testFunction(filePath, testNumber++, expectedValid);
			}
		}
		closedir(dir);
	} else
		std::cerr << "Could not open directory: " << folderPath << std::endl;
}


int main()
{
	std::cout << "\n****************** ServerConfig Tests **********************\n" << std::endl;

	int testNumber = 1;

	testFolder(TESTFOLDER + "Pass", testNumber, true);
	testFolder(TESTFOLDER + "Fail", testNumber, false);

	std::cout << "\n****************** *************** **********************\n" << std::endl;

	return 0;
}
