#include <iostream>
#include <string>
#include <stdexcept>  // For std::exception, std::runtime_error, etc.
#include <dirent.h>

#include "../ServerConfig/ServerConfig.hpp"
#include "../ServerBlock/ServerBlock.hpp"
#include "../ServerLocation/ServerLocation.hpp"
#include "../DefaultConfig/DefaultConfig.hpp"

#define TESTFOLDER std::string("./") 
#define CLR_BLUE "\033[34m"
#define CLR_RESET "\033[0m"
#define CLR_PASS "\033[32mPass\033[0m"	
#define CLR_FAIL "\033[31mFail\033[0m"

void testPass(const std::string &filePath, int testNbr, int printDetails)
{
	std::cout << CLR_BLUE << "Test " << testNbr << " : " << filePath << " " << CLR_RESET << std::endl;
	DefaultConfig defaultConfig;
	ServerConfig config(filePath.c_str(), defaultConfig);
	if (config.parseConfigFile())
	{
		std::cout << CLR_PASS << std::endl;
		if (printDetails)
			config.printConfigs();
	}
	else
		std::cout << CLR_FAIL << std::endl;
}

void testFail(const std::string &filePath, int testNbr, int printDetails)
{
	std::cout << CLR_BLUE << "Test " << testNbr << " : " << filePath << " " << CLR_RESET << std::endl;
	DefaultConfig defaultConfig;
	ServerConfig config(filePath.c_str(), defaultConfig);
	if (config.parseConfigFile())
	{
		std::cout << CLR_FAIL << std::endl;
		if (printDetails)
			config.printConfigs();
	}
	else
		std::cout << CLR_PASS << std::endl;
}

void testNoThrow(const std::string &filePath, int testNbr, int printDetails)
{
	std::cout << CLR_BLUE << "Test " << testNbr << " : " << filePath << " " << CLR_RESET << std::endl;
	try {
		DefaultConfig defaultConfig;
		ServerConfig config(filePath.c_str(), defaultConfig);
		config.parseConfigFile();
		if (printDetails)
			config.printConfigs();
		std::cout << "Passed!" << std::endl;
	}
	catch (std::exception &e) {
		std::cerr << "Test " << testNbr << " Failed: " << e.what() << std::endl;
	}
}

void testThrow(const std::string &filePath, int testNbr, int printDetails)
{
	std::cout << CLR_BLUE << "Test " << testNbr << " : " << filePath << " " << CLR_RESET << std::endl;
	try {
		DefaultConfig defaultConfig;
		ServerConfig config(filePath.c_str(), defaultConfig);
		config.parseConfigFile();
		if (printDetails)
			config.printConfigs();
		std::cerr << "Failed: Test should throw but didn't" << std::endl;
	}
	catch (std::exception &e) {
		std::cout << "Passed (throw): " << e.what() << std::endl;
	}
}

void testNoThrowInFolder(const std::string &folderPath, int printDetails)
{
	DIR		*dir;
	struct	dirent *ent;
	int		testNbr = 0;

	if ((dir = opendir(folderPath.c_str())) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_type == DT_REG) {
				std::string filePath = folderPath + "/" + ent->d_name;
				testNoThrow(filePath, ++testNbr, printDetails);
			}
		}
		closedir(dir);
	} else
		std::cerr << "Could not open directory: " << folderPath << std::endl;
}

void testThrowInFolder(const std::string &folderPath, int printDetails)
{
	DIR		*dir;
	struct	dirent *ent;
	int		testNbr = 0;

	if ((dir = opendir(folderPath.c_str())) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_type == DT_REG) {
				std::string filePath = folderPath + "/" + ent->d_name;
				testThrow(filePath, ++testNbr, printDetails);
			}
		}
		closedir(dir);
	} else
		std::cerr << "Could not open directory: " << folderPath << std::endl;
}

void testFolder(const std::string &folderPath, int printDetails, void (*testFunc)(const std::string &, int, int))
{
	DIR		*dir;
	struct	dirent *ent;
	int		testNbr = 0;

	if ((dir = opendir(folderPath.c_str())) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (ent->d_type == DT_REG) {
				std::string filePath = folderPath + "/" + ent->d_name;
				testFunc(filePath, ++testNbr, printDetails);
			}
		}
		closedir(dir);
	} else
		std::cerr << "Could not open directory: " << folderPath << std::endl;
}


int main()
{
	int printDetails;
	int tests;

	std::cout << "=== Testing ServerConfig ===" << std::endl;
	std::cout << "- Pass means that the output value is the expected value. Fail Means it isnt." << std::endl;
	std::cout << "- Wether the value is correctly stored needs to be checked manually by printing the stored information." << std::endl;
	std::cout << "- Enter 0 to do all tests, 1 to do the valid situations, 2 to do the invalid situations, 3 for custom tests" << std::endl;
	std::cin >> tests;
	std::cout << "- Enter 0 to see only the results or 1 to print stored information" << std::endl;
	std::cin >> printDetails;
	std::cout << "=== Testing ServerConfig ===" << std::endl;

	switch (tests) {
        case 0:
			std::cout << std::endl << "=== VALID TESTS ===" << std::endl;
			testFolder(TESTFOLDER + "Pass", printDetails, testPass);
			std::cout << std::endl << "=== INVALID TESTS ===" << std::endl;
			testFolder(TESTFOLDER + "Fail", printDetails, testFail);
			break;
		case 1:
			std::cout << std::endl << "=== VALID TESTS ===" << std::endl;
			testFolder(TESTFOLDER + "Pass", printDetails, testPass);
			break;
		case 2:
			std::cout << std::endl << "=== INVALID TESTS ===" << std::endl;
			testFolder(TESTFOLDER + "Fail", printDetails, testFail);
			break;
		case 3:
			std::cout << std::endl << "=== CUSTOM TESTS ===" << std::endl;
			testFolder(TESTFOLDER + "Custom", printDetails, testPass);
			break;
	}
	return 0;
}