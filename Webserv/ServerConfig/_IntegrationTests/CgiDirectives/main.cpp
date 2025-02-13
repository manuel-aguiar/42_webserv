
# include "../../DefaultConfig/DefaultConfig.hpp"
# include "../../ServerConfig/ServerConfig.hpp"
# include "../../ServerBlock/ServerBlock.hpp"
# include "../../ServerLocation/ServerLocation.hpp"
# include "../../../GenericUtils/WsTestHelpers/WsTestHelpers.h"
# include "../../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
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

////////////////////////////////////////////////////////////////////////
    configPath = "LocationOverride.conf";
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
        EXPECT_EQUAL(locationInterpreters.begin()->second, "tretas", "wrong match");

        TEST_PASSED_MSG(configPath + ": " + "Location cgi, ovewritting the global interpretor for this extension");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}


////////////////////////////////////////////////////////////////////////
    configPath = "LocationOverrideItself.conf";
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
        EXPECT_EQUAL(locationInterpreters.begin()->second, "coisas", "wrong match");

        TEST_PASSED_MSG(configPath + ": " + "Location cgi, ovewritting itself");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}
/////////////////////////////////////////////////////////////
//          FAILING CONFIGS NOW
//
//std::cerr expected here, diverting:

char buffer[1024];
int pipefd[2];
int dupstrerr = dup(STDERR_FILENO);
pipe(pipefd);
dup2(pipefd[1], STDERR_FILENO);
FileDescriptor::setNonBlocking(pipefd[0]);
FileDescriptor::setNonBlocking(pipefd[1]);


///////////////////////////////////////////////////////////
    while(read(pipefd[0], buffer, 1024) != -1);
//////////////////////////////////////////////////////////

    configPath = "ErrorLocation1.conf";
    try
    {
        TEST_INTRO(testNumber++);
        ServerConfig config(configPath.c_str(), defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), false, "Parsing must fail here");

        TEST_PASSED_MSG(configPath + ": " + "[extension]:");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}
///////////////////////////////////////////////////////////
    while(read(pipefd[0], buffer, 1024) != -1);
//////////////////////////////////////////////////////////

    configPath = "ErrorLocation2.conf";
    try
    {
        TEST_INTRO(testNumber++);
        ServerConfig config(configPath.c_str(), defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), false, "Parsing must fail here");

        TEST_PASSED_MSG(configPath + ": " + "[extension]");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}

///////////////////////////////////////////////////////////
    while(read(pipefd[0], buffer, 1024) != -1);
//////////////////////////////////////////////////////////

    configPath = "ErrorLocation3.conf";
    try
    {
        TEST_INTRO(testNumber++);
        ServerConfig config(configPath.c_str(), defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), false, "Parsing must fail here");

        TEST_PASSED_MSG(configPath + ": " + "[]:[]");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}

///////////////////////////////////////////////////////////
    while(read(pipefd[0], buffer, 1024) != -1);
//////////////////////////////////////////////////////////

    configPath = "ErrorLocation4.conf";
    try
    {
        TEST_INTRO(testNumber++);
        ServerConfig config(configPath.c_str(), defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), false, "Parsing must fail here");

        TEST_PASSED_MSG(configPath + ": " + ":[path]");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}

///////////////////////////////////////////////////////////
    while(read(pipefd[0], buffer, 1024) != -1);
//////////////////////////////////////////////////////////

    configPath = "ErrorGlobal1.conf";
    try
    {
        TEST_INTRO(testNumber++);
        ServerConfig config(configPath.c_str(), defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), false, "Parsing must fail here");

        TEST_PASSED_MSG(configPath + ": " + "[]:[]");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}

///////////////////////////////////////////////////////////
    while(read(pipefd[0], buffer, 1024) != -1);
//////////////////////////////////////////////////////////

    configPath = "ErrorGlobal2.conf";
    try
    {
        TEST_INTRO(testNumber++);
        ServerConfig config(configPath.c_str(), defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), false, "Parsing must fail here");

        TEST_PASSED_MSG(configPath + ": " + "global [extension]:[]");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}

///////////////////////////////////////////////////////////
    while(read(pipefd[0], buffer, 1024) != -1);
//////////////////////////////////////////////////////////

    configPath = "ErrorGlobal3.conf";
    try
    {
        TEST_INTRO(testNumber++);
        ServerConfig config(configPath.c_str(), defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), false, "Parsing must fail here");

        TEST_PASSED_MSG(configPath + ": " + "global overrides itself with bad value");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}
///////////////////////////////////////////////////////////
    while(read(pipefd[0], buffer, 1024) != -1);
//////////////////////////////////////////////////////////

    configPath = "ErrorGlobal4.conf";
    try
    {
        TEST_INTRO(testNumber++);
        ServerConfig config(configPath.c_str(), defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), false, "Parsing must fail here");

        TEST_PASSED_MSG(configPath + ": " + "[]:[path]");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}

///////////////////////////////////////////////////////////
    while(read(pipefd[0], buffer, 1024) != -1);
//////////////////////////////////////////////////////////
// restoring stderr
dup2(dupstrerr, STDERR_FILENO);
close(dupstrerr);
close(pipefd[0]);
close(pipefd[1]);
close(STDERR_FILENO);


////////////////////////////////////////////////////////////

    TEST_FOOTER;

    return (0);
}

