#ifndef TESTDEPENDENCIES_HPP
# define TESTDEPENDENCIES_HPP

# include "../../../GenericUtils/Webserver_Definitions.h"
# include <string>
# include <set>
# include <vector>

class ServerBlock {
    public:
        ServerBlock();
        explicit ServerBlock(const std::string& id);
        ~ServerBlock();

        ServerBlock(const ServerBlock& other);
        ServerBlock& operator=(const ServerBlock& other);

        std::string id() const;
        const std::vector<const struct sockaddr*>& getListenAddresses() const;
        const std::set<std::string>& getServerNames() const;

        void addListenAddress(const struct sockaddr* addr);
        void addServerName(const std::string& name);

    private:
        std::string m_id;
        std::vector<const struct sockaddr*> m_listenAddresses;
        std::set<std::string> m_server_names;
};


#endif
