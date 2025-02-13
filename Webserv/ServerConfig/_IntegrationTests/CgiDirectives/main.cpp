
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

    try
    {
        TEST_INTRO(testNumber++);

        ServerConfig config("GlobalDirective.conf", defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), true, "Should parse without issues");

        EXPECT_EQUAL(config.getMaxConnections(), 100, "Wrong max connections");

        const std::map<Config::CgiExtension, Config::CgiInterpreter>& 
        cgiInterpreters = config.getCgiInterpreters();

        EXPECT_EQUAL(cgiInterpreters.size(), 1, "Wrong number of cgi interpreters");
        EXPECT_EQUAL(cgiInterpreters.begin()->first, ".cgi", "wrong match");
        EXPECT_EQUAL(cgiInterpreters.begin()->second, "/cgi/cgi/cgi", "wrong match");

        TEST_PASSED_MSG("simple global directive");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
////////////////////////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);

        ServerConfig config("GlobalOverride.conf", defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), true, "Should parse without issues");

        EXPECT_EQUAL(config.getMaxConnections(), 100, "Wrong max connections");

        const std::map<Config::CgiExtension, Config::CgiInterpreter>& 
        cgiInterpreters = config.getCgiInterpreters();

        EXPECT_EQUAL(cgiInterpreters.size(), 1, "Wrong number of cgi interpreters");
        EXPECT_EQUAL(cgiInterpreters.begin()->first, ".cgi", "wrong match");
        EXPECT_EQUAL(cgiInterpreters.begin()->second, "/potato", "wrong match");

        TEST_PASSED_MSG("global directive override");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

////////////////////////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);

        ServerConfig config("GlobalSameLineOverride.conf", defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), true, "Should parse without issues");

        EXPECT_EQUAL(config.getMaxConnections(), 100, "Wrong max connections");

        const std::map<Config::CgiExtension, Config::CgiInterpreter>& 
        cgiInterpreters = config.getCgiInterpreters();

        EXPECT_EQUAL(cgiInterpreters.size(), 1, "Wrong number of cgi interpreters");
        EXPECT_EQUAL(cgiInterpreters.begin()->first, ".cgi", "wrong match");
        EXPECT_EQUAL(cgiInterpreters.begin()->second, "/potato", "wrong match");

        TEST_PASSED_MSG("global same line override");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}


////////////////////////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);

        ServerConfig config("GlobalMultiple.conf", defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), true, "Should parse without issues");

        EXPECT_EQUAL(config.getMaxConnections(), 100, "Wrong max connections");

        const std::map<Config::CgiExtension, Config::CgiInterpreter>& 
        cgiInterpreters = config.getCgiInterpreters();

        EXPECT_EQUAL(cgiInterpreters.size(), 2, "Wrong number of cgi interpreters");
        EXPECT_EQUAL(cgiInterpreters.begin()->first, ".cgi", "wrong match");
        EXPECT_EQUAL(cgiInterpreters.begin()->second, "/potato", "wrong match");

        EXPECT_EQUAL(cgiInterpreters.rbegin()->first, ".py", "wrong match");
        EXPECT_EQUAL(cgiInterpreters.rbegin()->second, "/usr/bin/python3", "wrong match");

        TEST_PASSED_MSG("global multiple interpreters");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

////////////////////////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);

        ServerConfig config("Location.conf", defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), true, "Should parse without issues");

        EXPECT_EQUAL(config.getMaxConnections(), 100, "Wrong max connections");

        const std::map<Config::CgiExtension, Config::CgiInterpreter>& 
        cgiInterpreters = config.getCgiInterpreters();

        EXPECT_EQUAL(cgiInterpreters.size(), 2, "Wrong number of cgi interpreters");
        EXPECT_EQUAL(cgiInterpreters.begin()->first, ".cgi", "wrong match");
        EXPECT_EQUAL(cgiInterpreters.begin()->second, "/potato", "wrong match");

        EXPECT_EQUAL(cgiInterpreters.rbegin()->first, ".py", "wrong match");
        EXPECT_EQUAL(cgiInterpreters.rbegin()->second, "/usr/bin/python3", "wrong match");

        const std::map<Config::CgiExtension, Config::CgiInterpreter>& 
        locationInterpreters = config.getServerBlocks()[0].getLocations().begin()->second.getCgiInterpreters();

        EXPECT_EQUAL(locationInterpreters.size(), 1, "Wrong number of cgi interpreters");
        EXPECT_EQUAL(locationInterpreters.begin()->first, ".cgi", "wrong match");
        EXPECT_EQUAL(locationInterpreters.begin()->second, "/potato", "wrong match");

        TEST_PASSED_MSG("Location cgi, resolving to global");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

    return (0);
}

