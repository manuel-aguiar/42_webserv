

#include "ModifyConnection.hpp"
#include "../../Globals/Globals.hpp"
#include "../../Events/Manager/Manager.hpp"
#include "../../ServerContext/ServerContext.hpp"

#include <iostream>

Ws::Sock::addr_in 	createSockAddr_in(const std::string& ip, const std::string& port);

void function(Conn::Connection& connection)
{
    std::cout << connection.info_getFd() << std::endl;    
}

/*
    Takes a bit of work because of all the dependencies but at least you don't need
    the InternalManager implementation to test a protocol.

    You can use ModifyConnection to modify the internals and pass it to your functions,
    the compiler will pass the Conn::Connection part of ModifyConnection

    But well, within this, you have all you need to use a connection, it has
    the event functions, access to the server context, and the socket info.
*/

int main(void)
{
    Globals globals;
    ServerContext ctx;
    Events::Manager eventManager(10, globals);

    ModifyConnection conn(eventManager, ctx);
    
    conn.setConnInfo(ConnInfo());
    ConnInfo externalConnect;
    ConnInfo externalConnect = 
    (ConnInfo){ 123,
        (Ws::BindInfo)
        {
            .appLayer = Ws::AppLayer::HTTP,
            .backlog = 128,
            .family = AF_INET,
            .socktype = SOCK_STREAM,
            .proto = IPPROTO_TCP,
            .addr = (Ws::Sock::union_addr){.sockaddr_in = createSockAddr_in("127.0.0.1", "8080")},
            .addrlen = sizeof(Ws::Sock::addr_in)
        }
    };

    function(conn);

    return (0);
}