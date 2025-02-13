
# include "../../DefaultConfig/DefaultConfig.hpp"
# include "../../ServerConfig/ServerConfig.hpp"
# include "../../ServerBlock/ServerBlock.hpp"
# include "../../ServerLocation/ServerLocation.hpp"
# include "../../../GenericUtils/WsTestHelpers/WsTestHelpers.h"
# include <iostream>
# include <arpa/inet.h>
# include <unistd.h>


int main(void)
{
    TEST_HEADER("Integration ServerConfig - CgiDirectives");

    int testNumber = 1;
///////////////////////////////////////////////////////////////////////////////////////

    DefaultConfig defaultConfig;
    std::string configPath;

    configPath = "GlobalDirective.conf";
    try
    {
        TEST_INTRO(testNumber++);
        ServerConfig config(configPath.c_str(), defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), true, "Should parse without issues");

        EXPECT_EQUAL(config.getMaxConnections(), 100, "Wrong max connections");

        const Config::CgiInterpreterMap& 
        cgiInterpreters = config.getCgiInterpreters();

        EXPECT_EQUAL(cgiInterpreters.size(), 1, "Wrong number of cgi interpreters");
        EXPECT_EQUAL(cgiInterpreters.begin()->first, ".cgi", "wrong match");
        EXPECT_EQUAL(cgiInterpreters.begin()->second, "/cgi/cgi/cgi", "wrong match");

        TEST_PASSED_MSG(configPath + ": " + "simple global directive");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}
////////////////////////////////////////////////////////////////////////
    configPath = "GlobalOverride.conf";
    try
    {
        TEST_INTRO(testNumber++);
        ServerConfig config(configPath.c_str(), defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), true, "Should parse without issues");

        EXPECT_EQUAL(config.getMaxConnections(), 100, "Wrong max connections");

        const Config::CgiInterpreterMap& 
        cgiInterpreters = config.getCgiInterpreters();

        EXPECT_EQUAL(cgiInterpreters.size(), 1, "Wrong number of cgi interpreters");
        EXPECT_EQUAL(cgiInterpreters.begin()->first, ".cgi", "wrong match");
        EXPECT_EQUAL(cgiInterpreters.begin()->second, "/potato", "wrong match");

        TEST_PASSED_MSG(configPath + ": " + "global directive override");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}

////////////////////////////////////////////////////////////////////////
    configPath = "GlobalSameLineOverride.conf";
    try
    {
        TEST_INTRO(testNumber++);
        ServerConfig config(configPath.c_str(), defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), true, "Should parse without issues");

        EXPECT_EQUAL(config.getMaxConnections(), 100, "Wrong max connections");

        const Config::CgiInterpreterMap& 
        cgiInterpreters = config.getCgiInterpreters();

        EXPECT_EQUAL(cgiInterpreters.size(), 1, "Wrong number of cgi interpreters");
        EXPECT_EQUAL(cgiInterpreters.begin()->first, ".cgi", "wrong match");
        EXPECT_EQUAL(cgiInterpreters.begin()->second, "/potato", "wrong match");

        TEST_PASSED_MSG(configPath + ": " + "global same line override");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}


////////////////////////////////////////////////////////////////////////
    configPath = "GlobalMultiple.conf";
    try
    {
        TEST_INTRO(testNumber++);
        ServerConfig config(configPath.c_str(), defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), true, "Should parse without issues");

        EXPECT_EQUAL(config.getMaxConnections(), 100, "Wrong max connections");

        const Config::CgiInterpreterMap& 
        cgiInterpreters = config.getCgiInterpreters();

        EXPECT_EQUAL(cgiInterpreters.size(), 2, "Wrong number of cgi interpreters");
        EXPECT_EQUAL(cgiInterpreters.begin()->first, ".cgi", "wrong match");
        EXPECT_EQUAL(cgiInterpreters.begin()->second, "/potato", "wrong match");

        EXPECT_EQUAL(cgiInterpreters.rbegin()->first, ".py", "wrong match");
        EXPECT_EQUAL(cgiInterpreters.rbegin()->second, "/usr/bin/python3", "wrong match");

        TEST_PASSED_MSG(configPath + ": " + "global multiple interpreters");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}

////////////////////////////////////////////////////////////////////////
    configPath = "LocationInherit.conf";
    try
    {
        TEST_INTRO(testNumber++);
        ServerConfig config(configPath.c_str(), defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), true, "Should parse without issues");

        EXPECT_EQUAL(config.getMaxConnections(), 100, "Wrong max connections");

        const Config::CgiInterpreterMap& 
        cgiInterpreters = config.getCgiInterpreters();

        EXPECT_EQUAL(cgiInterpreters.size(), 2, "Wrong number of cgi interpreters");
        EXPECT_EQUAL(cgiInterpreters.begin()->first, ".cgi", "wrong match");
        EXPECT_EQUAL(cgiInterpreters.begin()->second, "/potato", "wrong match");

        EXPECT_EQUAL(cgiInterpreters.rbegin()->first, ".py", "wrong match");
        EXPECT_EQUAL(cgiInterpreters.rbegin()->second, "/usr/bin/python3", "wrong match");

        EXPECT_EQUAL(config.getServerBlocks().size(), 1, "Wrong number of server blocks");
        EXPECT_EQUAL(config.getServerBlocks()[0].getLocations().size(), 1, "Wrong number of locations");
        
        const Config::CgiInterpreterMap& 
        locationInterpreters = config.getServerBlocks()[0].getLocations()[0].getCgiInterpreters();

        EXPECT_EQUAL(locationInterpreters.size(), 1, "Wrong number of cgi interpreters");
        EXPECT_EQUAL(locationInterpreters.begin()->first, ".py", "wrong match");
        EXPECT_EQUAL(locationInterpreters.begin()->second, "/usr/bin/python3", "wrong match");

        TEST_PASSED_MSG(configPath + ": " + "Location cgi, resolving to global");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}

    return (0);
}

