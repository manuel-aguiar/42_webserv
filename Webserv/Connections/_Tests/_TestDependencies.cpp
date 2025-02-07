

// Prooject headers
# include "_TestDependencies.hpp"
# include "_TestClientManager.tpp"
# include "../ConnInfo/ConnInfo.hpp"
# include "../../Events/Manager/Manager.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../../Toolkit/TestHelpers/TestHelpers.h"

// C headers
#include <unistd.h>     // For close()
#include <sys/socket.h> // For socket(), connect()
#include <arpa/inet.h>  // For inet_pton()

//C++ headers
#include <vector>
#include <cerrno>




Ws::Sock::addr_in createSockAddr_in(const std::string& ip, const std::string& port)
{
    Ws::Sock::addr_in addr = (Ws::Sock::addr_in){};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(std::atoi(port.c_str()));

    if (ip.empty() || ip == "0.0.0.0")
        addr.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0
    else
        inet_pton(AF_INET, ip.c_str(), &(addr.sin_addr));

    return (addr);
}

void    prepareBindAddresses(std::vector<Ws::BindInfo>& bindAddresses, const size_t countListeners, const int portStart)
{
    bindAddresses.resize(countListeners);
    for (size_t i = 0; i < bindAddresses.size(); ++i)
    {
        bindAddresses[i] = (Ws::BindInfo)
        {
            .appLayer = Ws::AppLayer::HTTP,
			.backlog = 128,
            .family = AF_INET,
            .socktype = SOCK_STREAM,
            .proto = IPPROTO_TCP,
            .addr = (Ws::Sock::union_addr){.sockaddr_in = createSockAddr_in("0.0.0.0", TestHelpers::to_string(portStart + i))},
            .addrlen = sizeof(Ws::Sock::addr_in)
        };
    }
}