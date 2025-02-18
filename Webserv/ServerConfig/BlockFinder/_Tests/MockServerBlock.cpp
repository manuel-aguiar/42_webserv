

#include "../../ServerBlock/ServerBlock.hpp"
#include "../../../Ws_Namespace.h"
#include "../../../GenericUtils/Validation/Validation.hpp"
#include "../../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../ServerLocation/ServerLocation.hpp"
#include "../../DefaultConfig/DefaultConfig.hpp"


// C++ headers

#include <cstdlib> // atoi 
//chatgpt, just getters

ServerBlock::ServerBlock() {}
ServerBlock::~ServerBlock() {}
ServerBlock::ServerBlock(const ServerBlock &other) : m_listen(other.m_listen), m_server_name(other.m_server_name), m_http_maxClientBodySize(other.m_http_maxClientBodySize), m_http_maxClientHeaderSize(other.m_http_maxClientHeaderSize), m_root(other.m_root), m_error_pages(other.m_error_pages), m_locations(other.m_locations), m_myListenAddresses(other.m_myListenAddresses) {}
ServerBlock& ServerBlock::operator=(const ServerBlock &other) { 
	if (this != &other) {
		m_listen = other.m_listen;
		m_server_name = other.m_server_name;
		m_http_maxClientBodySize = other.m_http_maxClientBodySize;
		m_http_maxClientHeaderSize = other.m_http_maxClientHeaderSize;
		m_root = other.m_root;
		m_error_pages = other.m_error_pages;
		m_locations = other.m_locations;
		m_myListenAddresses = other.m_myListenAddresses;
	}
	return *this; 
}
const std::set<std::string>& ServerBlock::getDomainNames() const { return m_server_name; }
const std::vector<ServerLocation>& ServerBlock::getLocations() const { return m_locations; }
void ServerBlock::mapLocations() {  }
void ServerBlock::setRootPath(const std::string &value) { m_root = value; }
void ServerBlock::setClientBodySize(const std::string &value) { m_http_maxClientBodySize = std::atoi(value.c_str()); }
void ServerBlock::setClientHeaderSize(const std::string &value) { m_http_maxClientHeaderSize = std::atoi(value.c_str()); }
void ServerBlock::addListener(const std::string &value) { (void)value; }
void ServerBlock::addServerName(const std::string &value) { m_server_name.insert(value); }
void ServerBlock::addErrorPage(const std::string &value) { (void)value; }

const std::set<Config::Listen>& ServerBlock::getListeners() const { return m_listen; }
const std::set<std::string>& ServerBlock::getServerNames() const { return m_server_name; }
size_t ServerBlock::getClientBodySize() const { return m_http_maxClientBodySize;}
size_t ServerBlock::getClientHeaderSize() const { return m_http_maxClientHeaderSize; }
const std::map<int, std::string>& ServerBlock::getErrorPages() const { return m_error_pages; }
const std::string& ServerBlock::getRoot() const { return m_root; }
void ServerBlock::setDefaults(const DefaultConfig& config) {(void)config;}
void ServerBlock::addConfigValue(const std::string &key, const std::string &value) {(void)key; (void)value;}
bool ServerBlock::validate() const { return true; }
void ServerBlock::printServerConfig() const {}
void ServerBlock::addListenAddress(const struct sockaddr* addr) { m_myListenAddresses.push_back(addr); }
const std::vector<const struct sockaddr*>& ServerBlock::getListenAddresses() const { return m_myListenAddresses; }