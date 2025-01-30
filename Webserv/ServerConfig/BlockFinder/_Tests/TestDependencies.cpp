
# include "TestDependencies.hpp"

// Constructor implementations
ServerBlock::ServerBlock() : m_id("default") {}
ServerBlock::ServerBlock(const std::string& id) : m_id(id) {}
ServerBlock::~ServerBlock() {}

// Copy constructor
ServerBlock::ServerBlock(const ServerBlock& other) {
    *this = other;
}

// Copy assignment operator
ServerBlock& ServerBlock::operator=(const ServerBlock& other) {
    if (this == &other)
        return *this;
    m_id = other.m_id;
    m_listenAddresses = other.m_listenAddresses;
    m_server_names = other.m_server_names;
    return *this;
}

// Getter implementations
std::string ServerBlock::id() const {
    return m_id;
}

const std::vector<const struct sockaddr*>& ServerBlock::getListenAddresses() const {
    return m_listenAddresses;
}

const std::set<std::string>& ServerBlock::getServerNames() const {
    return m_server_names;
}

// Setter implementations
void ServerBlock::addListenAddress(const struct sockaddr* addr) {
    m_listenAddresses.push_back(addr);
}

void ServerBlock::addServerName(const std::string& name) {
    m_server_names.insert(name);
}

