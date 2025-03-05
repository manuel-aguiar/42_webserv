#include "../../../Toolkit/TestHelpers/TestHelpers.h"

#include "../ServerConfig/ServerConfig.hpp"
#include "../DefaultConfig/DefaultConfig.hpp"
#include "../ServerBlock/ServerBlock.hpp"
#include "../ServerLocation/ServerLocation.hpp"

#include <dirent.h>
#include <iostream>
#include <string>
#define TESTFOLDER "./"

// Tests all files in the Fail folder
void test_ServerConfig_Fail(int &testNbr)
{
	TEST_HEADER("ServerConfig Parsing - Fail Situations");
	
	std::string testFolder = TESTFOLDER;
	testFolder += "Fail/";
	DIR		*dir;
	struct	dirent *ent;

	if ((dir = opendir(testFolder.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			if (ent->d_type == DT_REG)
			{
				std::string filePath = testFolder + ent->d_name;
				TEST_INTRO(testNbr++);
				try
				{
					DefaultConfig defaultConfig;
					ServerConfig config(filePath.c_str(), defaultConfig);
					ASSERT_EQUAL(config.parseConfigFile(), false, "Failed to parse config file: " + filePath);
					TEST_PASSED_MSG(filePath);
				}
				catch (std::exception &e)
				{
					TEST_FAILED_MSG(filePath);
				}
			}
		}
		closedir(dir);
	}
	else
		std::cerr << "Could not open directory: " << testFolder << std::endl;
}

 
void test_ServerConfig_Pass(int &testNbr)
{
	TEST_HEADER("ServerConfig Parsing - Pass Situations");

	std::string testFolder = TESTFOLDER;
	testFolder += "Pass/";

	DIR		*dir;
	if ((dir = opendir(testFolder.c_str())) == NULL)
	{
		std::cerr << "Could not open directory: " << testFolder << std::endl;
		return ;
	}

	///////////////////////////////////

	{
		TEST_INTRO(testNbr++);
		std::string filePath = testFolder + "Pass_BareMinimum.conf";
		try
		{
			DefaultConfig defaultConfig;
			ServerConfig config(filePath.c_str(), defaultConfig);
			ASSERT_EQUAL(config.parseConfigFile(), true, "Failed to parse config file: " + filePath);
			ASSERT_EQUAL(config.getServerBlocks().size(), 1, "ServerBlock amount does not match: " + filePath);
			ASSERT_EQUAL(config.getServerBlocks().begin()->getListeners().size(), 1, "Listener amount does not match: " + filePath);
			ASSERT_EQUAL(config.getServerBlocks().begin()->getListeners().begin()->hostname, "0.0.0.0", "Listener hostname does not match: " + filePath);
			ASSERT_EQUAL(config.getServerBlocks().begin()->getListeners().begin()->port, "8080", "Listener port does not match: " + filePath);
			TEST_PASSED_MSG(filePath);
		}
		catch (std::exception &e)
		{
			TEST_FAILED_MSG(filePath);
		}
	}

	///////////////////////////////////

	{
		TEST_INTRO(testNbr++);
		std::string filePath = testFolder + "Pass_BareMinimumLocation.conf";
		try
		{
			DefaultConfig defaultConfig;
			ServerConfig config(filePath.c_str(), defaultConfig);
			ASSERT_EQUAL(config.parseConfigFile(), true, "Failed to parse config file: " + filePath);
			ASSERT_EQUAL(config.getServerBlocks().size(), 1, "ServerBlock amount does not match: " + filePath);
			ASSERT_EQUAL(config.getServerBlocks().begin()->getListeners().size(), 1, "Listener amount does not match: " + filePath);
			ASSERT_EQUAL(config.getServerBlocks().begin()->getListeners().begin()->hostname, "0.0.0.0", "Listener hostname does not match: " + filePath);
			ASSERT_EQUAL(config.getServerBlocks().begin()->getListeners().begin()->port, "80", "Listener port does not match: " + filePath);
			ASSERT_EQUAL(config.getServerBlocks().begin()->getLocations().size(), 1, "Location amount does not match: " + filePath);
			ASSERT_EQUAL(config.getServerBlocks().begin()->getLocations().begin()->getPath(), "/", "Location path does not match: " + filePath);
			TEST_PASSED_MSG(filePath);
		}
		catch (std::exception &e)
		{
			TEST_FAILED_MSG(filePath);
		}
	}

	///////////////////////////////////

	{
		TEST_INTRO(testNbr++);
		std::string filePath = testFolder + "Pass_everything.conf";
		try
		{
			DefaultConfig defaultConfig;
			ServerConfig config(filePath.c_str(), defaultConfig);

			ASSERT_EQUAL(config.parseConfigFile(), true, "Failed to parse config file: " + filePath);
			std::vector<ServerBlock>::const_iterator serverBlockIt = config.getServerBlocks().begin();
			std::vector<ServerLocation>::const_iterator serverLocationIt;

			// Server Config
			ASSERT_EQUAL(config.getMaxConnections(), 10, "Max connections does not match: " + filePath);
			ASSERT_EQUAL(config.getMaxConcurrentCgi(), 3, "Max concurrent CGI does not match: " + filePath);
			ASSERT_EQUAL(config.getMaxCgiBacklog(), 35, "Max CGI backlog does not match: " + filePath);
			ASSERT_EQUAL(config.getMaxWorkers(), 10, "Max workers does not match: " + filePath);
			ASSERT_EQUAL(config.getClientBodySize(), 1048576, "Client body size does not match: " + filePath);
			ASSERT_EQUAL(config.getClientHeaderSize(), 1048576, "Client header size does not match: " + filePath);
			ASSERT_EQUAL(config.getServerBlocks().size(), 4, "ServerBlock amount does not match: " + filePath);

			// Server Block 1
			ASSERT_EQUAL(serverBlockIt->getListeners().size(), 2, "Listener amount does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getListeners().begin()->hostname, "192.168.1.1", "Listener hostname does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getListeners().begin()->port, "8080", "Listener port does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getServerNames().size(), 2, "Server name amount does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getServerNames().find("webserv.com")->compare("webserv.com"), 0, "Server name webserv.com does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getServerNames().find("www.webserv.com")->compare("www.webserv.com"), 0, "Server name www.webserv.com does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getRoot(), "./TestFolder", "Root does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getClientBodySize(), 1048576, "Client body size does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getClientHeaderSize(), 1048576, "Client header size does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getErrorPages().size(), 2, "Error page amount does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getErrorPages().find(404)->second, "./", "Error page 404 does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getErrorPages().find(405)->second, "./", "Error page 405 does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getLocations().size(), 0, "Location amount does not match: " + filePath);
			
			serverBlockIt++;
			serverLocationIt = serverBlockIt->getLocations().begin();

			// Server Block 2
			ASSERT_EQUAL(serverBlockIt->getListeners().size(), 1, "Listener amount does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getListeners().begin()->hostname, "0.0.0.0", "Listener hostname does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getListeners().begin()->port, "8080", "Listener port does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getServerNames().size(), 2, "Server name amount does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getServerNames().find("servserv.com")->compare("servserv.com"), 0, "Server name servserv.com does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getServerNames().find("www.servserv.com")->compare("www.servserv.com"), 0, "Server name www.servserv.com does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getRoot(), "./TestFolder/one", "Root does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getClientBodySize(), 2097152, "Client body size does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getClientHeaderSize(), 2097152, "Client header size does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getLocations().size(), 2, "Location amount does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getPath(), "/images", "Location path does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getRoot(), "./TestFolder/one", "Location root does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getIndex(), "index.html", "Location index does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getMethods().size(), 3, "Method amount does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getUploadPath(), "./TestFolder/one/", "Upload path does not match: " + filePath);
			ASSERT_EQUAL((++serverLocationIt)->getPath(), "/locpath", "Location path does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getRoot(), "./TestFolder/one", "Location root does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getMethods().size(), 1, "Method amount does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getMethods().find("GET")->compare("GET"), 0, "Method GET does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getAutoIndex(), 1, "Auto index does not match: " + filePath);
			
			serverBlockIt++;
			serverLocationIt = serverBlockIt->getLocations().begin();

			// Server Block 3
			ASSERT_EQUAL(serverBlockIt->getListeners().size(), 1, "Listener amount does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getListeners().begin()->hostname, "192.168.1.56", "Listener hostname does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getListeners().begin()->port, "8080", "Listener port does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getLocations().size(), 2, "Location amount does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getServerNames().size(), 2, "Server name amount does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getServerNames().find("sdddd.com")->compare("sdddd.com"), 0, "Server name sdddd.com does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getServerNames().find("www.sdddddrv.com")->compare("www.sdddddrv.com"), 0, "Server name www.sdddddrv.com does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getRoot(), "./TestFolder/two", "Root does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getClientBodySize(), 3145728, "Client body size does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getClientHeaderSize(), 3145728, "Client header size does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getErrorPages().size(), 1, "Error page amount does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getErrorPages().find(404)->second, "./", "Error page 404 does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getLocations().size(), 2, "Location amount does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getPath(), "/sfdf", "Location path does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getRoot(), "./TestFolder/two", "Location root does not match: " + filePath);
			ASSERT_EQUAL((++serverLocationIt)->getPath(), "/fdsfsdfs", "Location path does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getRoot(), "./TestFolder/two", "Location root does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getMethods().size(), 1, "Method amount does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getMethods().find("POST")->compare("POST"), 0, "Method POST does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getAutoIndex(), 0, "Auto index does not match: " + filePath);

			serverBlockIt++;
			serverLocationIt = serverBlockIt->getLocations().begin();

			// Server Block 4
			ASSERT_EQUAL(serverBlockIt->getListeners().size(), 1, "Listener amount does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getListeners().begin()->hostname, "0.0.0.0", "Listener hostname does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getListeners().begin()->port, "96", "Listener port does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getServerNames().size(), 1, "Server name amount does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getServerNames().find("ilikeservers.com")->compare("ilikeservers.com"), 0, "Server name ilikeservers.com does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getRoot(), "./", "Root does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getClientBodySize(), 4194304, "Client body size does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getClientHeaderSize(), 4194304, "Client header size does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getErrorPages().size(), 1, "Error page amount does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getErrorPages().find(404)->second, "./", "Error page 404 does not match: " + filePath);
			ASSERT_EQUAL(serverBlockIt->getLocations().size(), 1, "Location amount does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getPath(), "/oldpath", "Location path does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getReturn().first, 301, "Return code does not match: " + filePath);
			ASSERT_EQUAL(serverLocationIt->getReturn().second, "./", "Return path does not match: " + filePath);
			
			
			

			
			// serverBlockIt++;
			TEST_PASSED_MSG(filePath);
		}
		catch (std::exception &e)
		{
			TEST_FAILED_MSG(filePath);
		}
	}

	///////////////////////////////////

	{
		TEST_INTRO(testNbr++);
		std::string filePath = testFolder + "Pass_twoListen.conf";
		try
		{
			DefaultConfig defaultConfig;
			ServerConfig config(filePath.c_str(), defaultConfig);
			ASSERT_EQUAL(config.parseConfigFile(), true, "Failed to parse config file: " + filePath);

			// Server Config
			ASSERT_EQUAL(config.getServerBlocks().size(), 1, "ServerBlock amount does not match: " + filePath);
			ASSERT_EQUAL(config.getServerBlocks().begin()->getListeners().size(), 2, "Listener amount does not match: " + filePath);
			ASSERT_EQUAL(config.getServerBlocks().begin()->getListeners().begin()->hostname, "0.0.0.0", "Listener hostname does not match: " + filePath);
			ASSERT_EQUAL(config.getServerBlocks().begin()->getListeners().begin()->port, "80", "Listener port does not match: " + filePath);
			ASSERT_EQUAL((++config.getServerBlocks().begin()->getListeners().begin())->port, "8080", "Listener port does not match: " + filePath);
			
			TEST_PASSED_MSG(filePath);
		}
		catch (std::exception &e)
		{
			TEST_FAILED_MSG(filePath);
		}
	}

	///////////////////////////////////

	closedir(dir);

	TEST_FOOTER;
}

int main()
{
	int testNbr = 0;

	test_ServerConfig_Fail(testNbr);
	test_ServerConfig_Pass(testNbr);

	TEST_FOOTER;
	return (0);
}
