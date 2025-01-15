#ifndef TESTDEPENDENCIES_HPP
# define TESTDEPENDENCIES_HPP

# include "../../../GenericUtils/Webserver_Definitions.h"
# include <string>
# include <set>

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
            m_listen = other.m_listen;
            m_server_name = other.m_server_name;
            return (*this);
        }

        // used for testing
        std::string id() const { return (m_id); }

        // actual ServerBlock functionality
        const std::set<t_listeners>& getListeners() const { return m_listen; }
        const std::set<std::string>& getServerNames() const { return m_server_name; }
        void addListener(const t_ip_str& ip, const t_port_str& port) {
            m_listen.insert(t_listeners(ip, port));
        }
        void addServerName(const std::string& name) {
            m_server_name.insert(name);
        }

    private:
        std::string m_id;
        std::set<t_listeners> m_listen;
        std::set<std::string> m_server_name;
};

#endif
