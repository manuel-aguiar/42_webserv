#ifndef TESTDEPENDENCIES_HPP
# define TESTDEPENDENCIES_HPP

# include "../../../GenericUtils/Webserver_Definitions.h"
# include <string>
# include <set>
# include <vector>

class ServerBlock {
    public:
        ServerBlock() : m_id("default") {}
        explicit ServerBlock(const std::string& id) : m_id(id) {}
        ~ServerBlock() {}

        ServerBlock(const ServerBlock& other) {
            *this = other;
        }

        ServerBlock& operator=(const ServerBlock& other) {
            if (this == &other)
                return (*this);
            m_id = other.m_id;
            m_listenAddresses = other.m_listenAddresses;
            m_server_names = other.m_server_names;
            return (*this);
        }

        // used for testing
        std::string id() const { return (m_id); }

        // actual ServerBlock functionality
        const std::vector<const struct sockaddr*>& getListenAddresses() const {
            return m_listenAddresses;
        }

        const std::set<std::string>& getServerNames() const {
            return m_server_names;
        }

        void addListenAddress(const struct sockaddr* addr) {
            m_listenAddresses.push_back(addr);
        }

        void addServerName(const std::string& name) {
            m_server_names.insert(name);
        }

    private:
        std::string m_id;
        std::vector<const struct sockaddr*> m_listenAddresses;
        std::set<std::string> m_server_names;
};

#endif
