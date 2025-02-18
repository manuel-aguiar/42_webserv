

# include "../../DefaultConfig/DefaultConfig.hpp"
# include "../../ServerConfig/ServerConfig.hpp"
# include "../../ServerBlock/ServerBlock.hpp"
# include "../../ServerLocation/ServerLocation.hpp"
# include "../../../GenericUtils/WsTestHelpers/WsTestHelpers.h"
# include "../../../GenericUtils/StringUtils/StringUtils.hpp"
# include <iostream>
# include <arpa/inet.h>
# include <unistd.h>

# include "../../../../Toolkit/TestHelpers/TestHelpers.h"

std::string ipToStr(const Ws::Sock::addr* addr)
{
    char ip[INET_ADDRSTRLEN];
    ::inet_ntop(AF_INET, &((Ws::Sock::addr_in*)addr)->sin_addr, ip, INET_ADDRSTRLEN);
    return (ip);
}

void testPart1(int& testNumber)
{
    ///////////////////////////////////////////////////////////////////////////////////////

    DefaultConfig defaultConfig;
    std::string configPath;

    configPath = "Pass_BareMinimum.conf";
    try
    {
        TEST_INTRO(testNumber++);
        ServerConfig config(configPath.c_str(), defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), true, "Should parse without issues");

        // defaults
        EXPECT_EQUAL(config.getMaxConnections(), defaultConfig.server_maxConnections, "Wrong max connections");
        EXPECT_EQUAL(config.getMaxConcurrentCgi(), defaultConfig.server_cgiWorkers, "Wrong max concurrent cgi");
        EXPECT_EQUAL(config.getMaxCgiBacklog(), defaultConfig.server_cgiBacklog, "Wrong max cgi backlog");
        EXPECT_EQUAL(config.getMaxWorkers(), defaultConfig.server_Workers, "Wrong max workers");
        EXPECT_EQUAL(config.getClientBodySize(), defaultConfig.http_maxClientBodySize, "Wrong client body size");
        EXPECT_EQUAL(config.getClientHeaderSize(), defaultConfig.http_maxClientHeaderSize, "Wrong client header size");
        EXPECT_EQUAL(config.getTimeoutFullHeader(), defaultConfig.http_timeoutFullHeader, "Wrong full header timeout");
        EXPECT_EQUAL(config.getTimeoutInterSend(), defaultConfig.http_timeoutInterSend, "Wrong inter send timeout");
        EXPECT_EQUAL(config.getTimeoutInterReceive(), defaultConfig.http_timeoutInterReceive, "Wrong inter receive timeout");


        // blocks
        const std::vector<ServerBlock>& serverBlocks = config.getServerBlocks();
        EXPECT_EQUAL(serverBlocks.size(), 1, "Wrong number of server blocks");
        
        // single block
        const ServerBlock& serverBlock = serverBlocks[0];
        const std::set<std::string>& serverNames = serverBlock.getServerNames();
        EXPECT_EQUAL(serverNames.size(), 1, "Wrong number of server names");
        EXPECT_EQUAL(*serverNames.begin(), "potato.com", "Wrong server name");

        //defaults
        EXPECT_EQUAL(serverBlock.getClientBodySize(), defaultConfig.http_maxClientBodySize, "Wrong client body size");
        EXPECT_EQUAL(serverBlock.getClientHeaderSize(), defaultConfig.http_maxClientHeaderSize, "Wrong client header size");
        EXPECT_EQUAL(serverBlock.getRoot(), defaultConfig.server_Root, "Wrong root path");

        // default bind addresses
        const std::vector<const Ws::Sock::addr*>& listenAddresses = serverBlock.getListenAddresses();
        EXPECT_EQUAL(listenAddresses.size(), 1, "Wrong number of listen addresses");
        EXPECT_EQUAL(ipToStr(listenAddresses[0]), defaultConfig.server_IPListen, "Wrong listen address");
        EXPECT_EQUAL(::ntohs(((Ws::Sock::addr_in*)listenAddresses[0])->sin_port), 80, "Wrong listen port");

        // default listeners
        const std::set<Config::Listen>& listeners = serverBlock.getListeners();
        EXPECT_EQUAL(listeners.size(), 1, "Wrong number of listeners");
        const Config::Listen& listen = *listeners.begin();
        EXPECT_EQUAL(listen.appLayer, Ws::AppLayer::HTTP, "Wrong app layer");
        EXPECT_EQUAL(listen.backlog, (int)Ws::Listen::DEFAULT_BACKLOG, "Wrong backlog");
        EXPECT_EQUAL(listen.port, "80", "Wrong port");
        EXPECT_EQUAL(listen.hostname, "0.0.0.0", "Wrong hostname");
        EXPECT_EQUAL(listen.family, AF_INET, "Wrong family");
        EXPECT_EQUAL(listen.socktype, SOCK_STREAM, "Wrong socktype");
        EXPECT_EQUAL(listen.proto, (int)IPPROTO_TCP, "Wrong proto");
    
        // cgi
        const Config::CgiInterpreterMap& 
        cgiInterpreters = config.getCgiInterpreters();
        EXPECT_EQUAL(cgiInterpreters.size(), 0, "Wrong number of cgi interpreters");

        TEST_PASSED_MSG(configPath + ": " + "basic config, checking defaults");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}

/////////////////////////////////////////////////////////////////
    configPath = "Pass_BasicSomeOverrides.conf";
    try
    {
        TEST_INTRO(testNumber++);
        ServerConfig config(configPath.c_str(), defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), true, "Should parse without issues");

        // defaults
        EXPECT_EQUAL(config.getMaxConnections(), defaultConfig.server_maxConnections, "Wrong max connections");
        EXPECT_EQUAL(config.getMaxConcurrentCgi(), defaultConfig.server_cgiWorkers, "Wrong max concurrent cgi");
        EXPECT_EQUAL(config.getMaxCgiBacklog(), defaultConfig.server_cgiBacklog, "Wrong max cgi backlog");
        EXPECT_EQUAL(config.getMaxWorkers(), defaultConfig.server_Workers, "Wrong max workers");
        EXPECT_EQUAL(config.getClientBodySize(), defaultConfig.http_maxClientBodySize, "Wrong client body size");
        EXPECT_EQUAL(config.getClientHeaderSize(), defaultConfig.http_maxClientHeaderSize, "Wrong client header size");
        EXPECT_EQUAL(config.getTimeoutFullHeader(), defaultConfig.http_timeoutFullHeader, "Wrong full header timeout");
        EXPECT_EQUAL(config.getTimeoutInterSend(), defaultConfig.http_timeoutInterSend, "Wrong inter send timeout");
        EXPECT_EQUAL(config.getTimeoutInterReceive(), defaultConfig.http_timeoutInterReceive, "Wrong inter receive timeout");

        // blocks
        const std::vector<ServerBlock>& serverBlocks = config.getServerBlocks();
        EXPECT_EQUAL(serverBlocks.size(), 1, "Wrong number of server blocks");
        
        // single block
        const ServerBlock& serverBlock = serverBlocks[0];
        const std::set<std::string>& serverNames = serverBlock.getServerNames();
        EXPECT_EQUAL(serverNames.size(), 2, "Wrong number of server names");
        EXPECT_EQUAL(*serverNames.begin(), "batata.com", "Wrong server name");
        EXPECT_EQUAL(*serverNames.rbegin(), "potato.com", "Wrong server name");

        //defaults
        EXPECT_EQUAL(serverBlock.getClientBodySize(), StringUtils::parse_size("1M"), "Wrong client body size");
        EXPECT_EQUAL(serverBlock.getClientHeaderSize(), defaultConfig.http_maxClientHeaderSize, "Wrong client header size");
        EXPECT_EQUAL(serverBlock.getTimeoutFullHeader(), defaultConfig.http_timeoutFullHeader, "Wrong full header timeout");
        EXPECT_EQUAL(serverBlock.getTimeoutInterSend(), defaultConfig.http_timeoutInterSend, "Wrong inter send timeout");
        EXPECT_EQUAL(serverBlock.getTimeoutInterReceive(), defaultConfig.http_timeoutInterReceive, "Wrong inter receive timeout");
        EXPECT_EQUAL(serverBlock.getRoot(), defaultConfig.server_Root, "Wrong root path");

        // default bind addresses
        const std::vector<const Ws::Sock::addr*>& listenAddresses = serverBlock.getListenAddresses();
        EXPECT_EQUAL(listenAddresses.size(), 2, "Wrong number of listen addresses");
        EXPECT_EQUAL(ipToStr(listenAddresses[0]), defaultConfig.server_IPListen, "Wrong listen address");
        EXPECT_EQUAL(::ntohs(((Ws::Sock::addr_in*)listenAddresses[0])->sin_port), 80, "Wrong listen port");
        EXPECT_EQUAL(ipToStr(listenAddresses[1]), "123.123.123.123", "Wrong listen address");
        EXPECT_EQUAL(::ntohs(((Ws::Sock::addr_in*)listenAddresses[1])->sin_port), 123, "Wrong listen port");

        // first listener
        const std::set<Config::Listen>& listeners = serverBlock.getListeners();
        EXPECT_EQUAL(listeners.size(), 2, "Wrong number of listeners");
        const Config::Listen& listen = *listeners.begin();
        EXPECT_EQUAL(listen.appLayer, Ws::AppLayer::HTTP, "Wrong app layer");
        EXPECT_EQUAL(listen.backlog, (int)Ws::Listen::DEFAULT_BACKLOG, "Wrong backlog");
        EXPECT_EQUAL(listen.port, "80", "Wrong port");
        EXPECT_EQUAL(listen.hostname, "0.0.0.0", "Wrong hostname");
        EXPECT_EQUAL(listen.family, AF_INET, "Wrong family");
        EXPECT_EQUAL(listen.socktype, SOCK_STREAM, "Wrong socktype");
        EXPECT_EQUAL(listen.proto, (int)IPPROTO_TCP, "Wrong proto");

        // second listener
        const Config::Listen& listen2 = *listeners.rbegin();
        EXPECT_EQUAL(listen2.appLayer, Ws::AppLayer::HTTP, "Wrong app layer");
        EXPECT_EQUAL(listen2.backlog, (int)Ws::Listen::DEFAULT_BACKLOG, "Wrong backlog");
        EXPECT_EQUAL(listen2.port, "123", "Wrong port");
        EXPECT_EQUAL(listen2.hostname, "123.123.123.123", "Wrong hostname");
        EXPECT_EQUAL(listen2.family, AF_INET, "Wrong family");
        EXPECT_EQUAL(listen2.socktype, SOCK_STREAM, "Wrong socktype");
        EXPECT_EQUAL(listen2.proto, (int)IPPROTO_TCP, "Wrong proto");


        //single location
        const std::vector<ServerLocation>& locations = serverBlock.getLocations();
        EXPECT_EQUAL(locations.size(), 1, "Wrong number of locations");
        EXPECT_EQUAL(locations[0].getRoot(), "/var/www/potato.com", "Wrong root path");
        EXPECT_EQUAL(locations[0].getPath(), "/var/www/potato.com", "Wrong path");
        EXPECT_EQUAL(locations[0].getAutoIndex(), false, "Wrong autoindex");

        // cgi, location level
        const Config::CgiInterpreterMap& locationCgi = locations[0].getCgiInterpreters();
        EXPECT_EQUAL(locationCgi.size(), 2, "Wrong number of cgi interpreters");
        EXPECT_EQUAL(locationCgi.begin()->first, ".php" , "Wrong cgi interpreter");
        EXPECT_EQUAL(locationCgi.begin()->second, "coisas", "Wrong cgi interpreter");
        EXPECT_EQUAL(locationCgi.rbegin()->first, ".py" , "Wrong cgi interpreter");
        EXPECT_EQUAL(locationCgi.rbegin()->second, "cenas", "Wrong cgi interpreter");

        // cgi, server level
        const Config::CgiInterpreterMap& 
        cgiInterpreters = config.getCgiInterpreters();
        EXPECT_EQUAL(cgiInterpreters.size(), 0, "Wrong number of cgi interpreters");

        TEST_PASSED_MSG(configPath + ": " + "basic config, checking defaults");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}

/////////////////////////////////////////////////////////////////
    configPath = "Pass_Multiple.conf";
    try
    {
        TEST_INTRO(testNumber++);
        ServerConfig config(configPath.c_str(), defaultConfig);

        EXPECT_EQUAL(config.parseConfigFile(), true, "Should parse without issues");

        // defaults
        EXPECT_EQUAL(config.getMaxConnections(), defaultConfig.server_maxConnections, "Wrong max connections");
        EXPECT_EQUAL(config.getMaxConcurrentCgi(), defaultConfig.server_cgiWorkers, "Wrong max concurrent cgi");
        EXPECT_EQUAL(config.getMaxCgiBacklog(), 420, "Wrong max cgi backlog");
        EXPECT_EQUAL(config.getMaxWorkers(), 10, "Wrong max workers");
        EXPECT_EQUAL(config.getClientBodySize(), 69420, "Wrong client body size");
        EXPECT_EQUAL(config.getClientHeaderSize(), defaultConfig.http_maxClientHeaderSize, "Wrong client header size");
        EXPECT_EQUAL(config.getTimeoutFullHeader(), defaultConfig.http_timeoutFullHeader, "Wrong full header timeout");
        EXPECT_EQUAL(config.getTimeoutInterSend(), defaultConfig.http_timeoutInterSend, "Wrong inter send timeout");
        EXPECT_EQUAL(config.getTimeoutInterReceive(), defaultConfig.http_timeoutInterReceive, "Wrong inter receive timeout");
        EXPECT_EQUAL(config.getTimeoutKeepAlive(), 2000, "Wrong keep alive timeout");

        // blocks
        const std::vector<ServerBlock>& serverBlocks = config.getServerBlocks();
        EXPECT_EQUAL(serverBlocks.size(), 2, "Wrong number of server blocks");

        // cgi, server level
        const Config::CgiInterpreterMap& 
        cgiInterpreters = config.getCgiInterpreters();
        EXPECT_EQUAL(cgiInterpreters.size(), 3, "Wrong number of cgi interpreters");
        Config::CgiInterpreterMap::const_iterator it = cgiInterpreters.begin();
        EXPECT_EQUAL(it->first, ".php", "Wrong cgi interpreter");
        EXPECT_EQUAL(it->second, "/usr/bin/php8.3", "Wrong cgi interpreter");
        it++;
        EXPECT_EQUAL(it->first, ".py", "Wrong cgi interpreter");
        EXPECT_EQUAL(it->second, "/usr/bin/python3", "Wrong cgi interpreter");
        it++;
        EXPECT_EQUAL(it->first, ".sh", "Wrong cgi interpreter");
        EXPECT_EQUAL(it->second, "/usr/bin/bash", "Wrong cgi interpreter");

    {
        //
        //          First Server Block
        //
        const ServerBlock& serverBlock = serverBlocks[0];
        const std::set<std::string>& serverNames = serverBlock.getServerNames();
        EXPECT_EQUAL(serverNames.size(), 2, "Wrong number of server names");
        EXPECT_EQUAL(*serverNames.begin(), "batata.com", "Wrong server name");
        EXPECT_EQUAL(*serverNames.rbegin(), "potato.com", "Wrong server name");

        //defaults
        EXPECT_EQUAL(serverBlock.getClientBodySize(), StringUtils::parse_size("1M"), "Wrong client body size");
        EXPECT_EQUAL(serverBlock.getClientHeaderSize(), defaultConfig.http_maxClientHeaderSize, "Wrong client header size");
        EXPECT_EQUAL(serverBlock.getTimeoutInterSend(), 1000, "Wrong timeout inter send");
        EXPECT_EQUAL(serverBlock.getTimeoutKeepAlive(), 3000, "Wrong timeout inter receive"); // overriden
        EXPECT_EQUAL(serverBlock.getRoot(), defaultConfig.server_Root, "Wrong root path");

        // default bind addresses
        const std::vector<const Ws::Sock::addr*>& listenAddresses = serverBlock.getListenAddresses();
        EXPECT_EQUAL(listenAddresses.size(), 2, "Wrong number of listen addresses");
        EXPECT_EQUAL(ipToStr(listenAddresses[0]), defaultConfig.server_IPListen, "Wrong listen address");
        EXPECT_EQUAL(::ntohs(((Ws::Sock::addr_in*)listenAddresses[0])->sin_port), 80, "Wrong listen port");
        EXPECT_EQUAL(ipToStr(listenAddresses[1]), "123.123.123.123", "Wrong listen address");
        EXPECT_EQUAL(::ntohs(((Ws::Sock::addr_in*)listenAddresses[1])->sin_port), 123, "Wrong listen port");

        // first listener
        const std::set<Config::Listen>& listeners = serverBlock.getListeners();
        EXPECT_EQUAL(listeners.size(), 2, "Wrong number of listeners");
        const Config::Listen& listen = *listeners.begin();
        EXPECT_EQUAL(listen.appLayer, Ws::AppLayer::HTTP, "Wrong app layer");
        EXPECT_EQUAL(listen.backlog, (int)Ws::Listen::DEFAULT_BACKLOG, "Wrong backlog");
        EXPECT_EQUAL(listen.port, "80", "Wrong port");
        EXPECT_EQUAL(listen.hostname, "0.0.0.0", "Wrong hostname");
        EXPECT_EQUAL(listen.family, AF_INET, "Wrong family");
        EXPECT_EQUAL(listen.socktype, SOCK_STREAM, "Wrong socktype");
        EXPECT_EQUAL(listen.proto, (int)IPPROTO_TCP, "Wrong proto");

        // second listener
        const Config::Listen& listen2 = *listeners.rbegin();
        EXPECT_EQUAL(listen2.appLayer, Ws::AppLayer::HTTP, "Wrong app layer");
        EXPECT_EQUAL(listen2.backlog, (int)Ws::Listen::DEFAULT_BACKLOG, "Wrong backlog");
        EXPECT_EQUAL(listen2.port, "123", "Wrong port");
        EXPECT_EQUAL(listen2.hostname, "123.123.123.123", "Wrong hostname");
        EXPECT_EQUAL(listen2.family, AF_INET, "Wrong family");
        EXPECT_EQUAL(listen2.socktype, SOCK_STREAM, "Wrong socktype");
        EXPECT_EQUAL(listen2.proto, (int)IPPROTO_TCP, "Wrong proto");


        //single location
        const std::vector<ServerLocation>& locations = serverBlock.getLocations();
        EXPECT_EQUAL(locations.size(), 1, "Wrong number of locations");
        EXPECT_EQUAL(locations[0].getRoot(), "/var/www/potato.com", "Wrong root path");
        EXPECT_EQUAL(locations[0].getPath(), "/var/www/potato.com", "Wrong path");
        EXPECT_EQUAL(locations[0].getAutoIndex(), false, "Wrong autoindex");

        // cgi, location level
        const Config::CgiInterpreterMap& locationCgi = locations[0].getCgiInterpreters();
        EXPECT_EQUAL(locationCgi.size(), 2, "Wrong number of cgi interpreters");
        EXPECT_EQUAL(locationCgi.begin()->first, ".php" , "Wrong cgi interpreter");
        EXPECT_EQUAL(locationCgi.begin()->second, "coisas", "Wrong cgi interpreter");
        EXPECT_EQUAL(locationCgi.rbegin()->first, ".py" , "Wrong cgi interpreter");
        EXPECT_EQUAL(locationCgi.rbegin()->second, "/usr/bin/python3", "Wrong cgi interpreter");
    }

    {
        //
        //          Second Server Block
        //
        const ServerBlock& serverBlock = serverBlocks[1];
        const std::set<std::string>& serverNames = serverBlock.getServerNames();
        EXPECT_EQUAL(serverNames.size(), 3, "Wrong number of server names");
        std::set<std::string>::const_iterator thisName = serverNames.begin();

        EXPECT_EQUAL(*thisName, "another.com", "Wrong server name"); thisName++;
        EXPECT_EQUAL(*thisName, "coisas.com", "Wrong server name"); thisName++;
        EXPECT_EQUAL(*thisName, "tretas.com", "Wrong server name");

        //defaults
        EXPECT_EQUAL(serverBlock.getClientBodySize(), 69420, "Wrong client header size");
        EXPECT_EQUAL(serverBlock.getClientHeaderSize(), StringUtils::parse_size("1K"), "Wrong client body size");
        EXPECT_EQUAL(serverBlock.getTimeoutKeepAlive(), 2000, "Wrong keep alive timeout"); // inherited
        EXPECT_EQUAL(serverBlock.getRoot(), defaultConfig.server_Root, "Wrong root path");

        // default bind addresses
        const std::vector<const Ws::Sock::addr*>& listenAddresses = serverBlock.getListenAddresses();
        EXPECT_EQUAL(listenAddresses.size(), 1, "Wrong number of listen addresses");
        EXPECT_EQUAL(ipToStr(listenAddresses[0]), "111.111.111.111", "Wrong listen address");
        EXPECT_EQUAL(::ntohs(((Ws::Sock::addr_in*)listenAddresses[0])->sin_port), 111, "Wrong listen port");

        // first listener
        const std::set<Config::Listen>& listeners = serverBlock.getListeners();
        EXPECT_EQUAL(listeners.size(), 1, "Wrong number of listeners");
        const Config::Listen& listen = *listeners.begin();
        EXPECT_EQUAL(listen.appLayer, Ws::AppLayer::HTTP, "Wrong app layer");
        EXPECT_EQUAL(listen.backlog, (int)Ws::Listen::DEFAULT_BACKLOG, "Wrong backlog");
        EXPECT_EQUAL(listen.port, "111", "Wrong port");
        EXPECT_EQUAL(listen.hostname, "111.111.111.111", "Wrong hostname");
        EXPECT_EQUAL(listen.family, AF_INET, "Wrong family");
        EXPECT_EQUAL(listen.socktype, SOCK_STREAM, "Wrong socktype");
        EXPECT_EQUAL(listen.proto, (int)IPPROTO_TCP, "Wrong proto");



        const std::vector<ServerLocation>& locations = serverBlocks[1].getLocations();
        EXPECT_EQUAL(locations.size(), 2, "Wrong number of locations");


        //
        //      First Location
        //        


    {
        const ServerLocation& location = locations[0];
        EXPECT_EQUAL(location.getRoot(), "goodbye", "Wrong root path");
        EXPECT_EQUAL(location.getPath(), "hello", "Wrong path");
        EXPECT_EQUAL(location.getAutoIndex(), true, "Wrong autoindex");

        // cgi, location level
        const Config::CgiInterpreterMap& locationCgi = location.getCgiInterpreters();
        EXPECT_EQUAL(locationCgi.size(), 2, "Wrong number of cgi interpreters");
        EXPECT_EQUAL(locationCgi.begin()->first, ".php" , "Wrong cgi interpreter");
        EXPECT_EQUAL(locationCgi.begin()->second, "/usr/bin/php8.2", "Wrong cgi interpreter");
        EXPECT_EQUAL(locationCgi.rbegin()->first, ".py" , "Wrong cgi interpreter");
        EXPECT_EQUAL(locationCgi.rbegin()->second, "cenas", "Wrong cgi interpreter");
    }

        //
        //      Second Location
        //        

    {
        const ServerLocation& location = locations[1];
        EXPECT_EQUAL(location.getRoot(), "byeAgain", "Wrong root path");
        EXPECT_EQUAL(location.getPath(), "heyAgain", "Wrong path");
        EXPECT_EQUAL(location.getAutoIndex(), false, "Wrong autoindex");

        // cgi, location level
        const Config::CgiInterpreterMap& locationCgi = location.getCgiInterpreters();
        EXPECT_EQUAL(locationCgi.size(), 2, "Wrong number of cgi interpreters");
        EXPECT_EQUAL(locationCgi.begin()->first, ".php" , "Wrong cgi interpreter");
        EXPECT_EQUAL(locationCgi.begin()->second, "/usr/bin/php8.1", "Wrong cgi interpreter");
        EXPECT_EQUAL(locationCgi.rbegin()->first, ".sh" , "Wrong cgi interpreter");
        EXPECT_EQUAL(locationCgi.rbegin()->second, "/usr/bin/bash", "Wrong cgi interpreter");
    }


    }   // end 2nd server block scope

        TEST_PASSED_MSG(configPath + ": " + "basic config, checking defaults");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(configPath + ": " + e.what());
	}

}