

// Project headers
# include "../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../Socket/Socket.hpp"

// C++ headers
#include <cstdlib>
#include <iostream>

// C headers
#include <arpa/inet.h>

Ws::Sock::addr_in createSockAddr_in(const std::string& ip, const std::string& port);

struct BindInfo_Internals : public Ws::BindInfo
{
    bool operator==(const BindInfo_Internals& rhs) const
    {
        return (appLayer == rhs.appLayer &&
                family == rhs.family &&
                socktype == rhs.socktype &&
                proto == rhs.proto &&
                addrlen == rhs.addrlen &&
                memcmp(&addr, &rhs.addr, addrlen) == 0);
    }
    bool operator!=(const BindInfo_Internals& rhs) const
    {
        return !(*this == rhs);
    };
};

std::ostream& operator<<(std::ostream& os, const BindInfo_Internals& info)
{
    os << "AppLayer: " << info.appLayer << std::endl;
    os << "Family: " << info.family << std::endl;
    os << "Socktype: " << info.socktype << std::endl;
    os << "Proto: " << info.proto << std::endl;
    os << "Addr: " << info.addr.sockaddr_in.sin_addr.s_addr << std::endl;
    os << "Port: " << info.addr.sockaddr_in.sin_port << std::endl;
    os << "Addrlen: " << info.addrlen << std::endl;

    return (os);
}

void testSocket(int& testNumber)
{

    try
    {
        TEST_INTRO(testNumber++);

        Socket          socket;

        socket.setSockFd(42);
        
        //test
        EXPECT_EQUAL(socket.getSockFd(), 42, "sockfd doesn't match");

        Ws::BindInfo& info = socket.modifyBindInfo();
    
        info = (Ws::BindInfo)
        {
            .appLayer = Ws::AppLayer::HTTP,
            .backlog = 128,
            .family = AF_INET,
            .socktype = SOCK_STREAM,
            .proto = IPPROTO_TCP,
            .addr = (Ws::Sock::union_addr){.sockaddr_in = createSockAddr_in("123.123.123.123", "8080")},
            .addrlen = sizeof(Ws::Sock::addr_in)
        };

        //test
        {
            const BindInfo_Internals& set_info = static_cast<const BindInfo_Internals&>(info);
            const BindInfo_Internals& get_info = static_cast<const BindInfo_Internals&>(socket.getBindInfo());

            EXPECT_EQUAL(get_info, set_info, "BindInfo doesn't match");
        }

        Ws::BindInfo    new_info = (Ws::BindInfo)
        {
            .appLayer = Ws::AppLayer::HTTP,
            .backlog = 128,
            .family = AF_INET,
            .socktype = SOCK_STREAM,
            .proto = IPPROTO_TCP,
            .addr = (Ws::Sock::union_addr){.sockaddr_in = createSockAddr_in("0.0.0.0", "9090")},
            .addrlen = sizeof(Ws::Sock::addr_in)
        };

        socket.setBindInfo(new_info);

        //test
        {
            const BindInfo_Internals& set_info = static_cast<const BindInfo_Internals&>(new_info);
            const BindInfo_Internals& get_info = static_cast<const BindInfo_Internals&>(socket.getBindInfo());
            EXPECT_EQUAL(get_info, set_info, "BindInfo doesn't match");
        }

        TEST_PASSED_MSG("Socket: tests passed");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
}