

# include "../../Ws_Namespace.h"

// C++ headers
#include <cstdlib>

// C headers
#include <arpa/inet.h>

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