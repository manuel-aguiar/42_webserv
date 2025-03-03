

#ifndef WS_NAMESPACE_H
# define WS_NAMESPACE_H

# include <stdint.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/un.h>
# include <netdb.h>

# include <string>

// forward declarations
namespace Conn { class Connection;}

namespace Ws
{
	#define WEBSERVER_VERSION_NAME "42_webserv/1.0"

	typedef int					fd;
	typedef unsigned char		byte;
	enum { FD_NONE = -1 };
	typedef pid_t				pid;
	
	typedef std::string			path;
	typedef std::string			strIP;
	typedef std::string			strPort;

	namespace Sock
	{
		typedef int					fd;
		typedef socklen_t			addrlen;
		typedef int					addrFamily;
		typedef int					type;
		typedef int					protocol;
		typedef struct addrinfo		Info;
		typedef struct sockaddr		addr;
		typedef struct sockaddr_in	addr_in;
		typedef struct sockaddr_in6	addr_in6;
		typedef struct sockaddr_un	addr_un;

		typedef union
		{
			Sock::addr         sockaddr;
			Sock::addr_in      sockaddr_in;
			Sock::addr_in6     sockaddr_in6;
			Sock::addr_un      sockaddr_un;
		}   union_addr;
	}

	namespace Listen
	{
		typedef int		backlog;

		enum { DEFAULT_BACKLOG = 128 };
	}

	namespace AppLayer
	{
		typedef enum
		{
			HTTP = 0,
			_FREE1,
			_FREE2,
			_FREE3,
			COUNT
		}	Type;
		typedef void* Module;
		typedef void* Conn;
		typedef void (*Init)(::Conn::Connection&);
		typedef void (*CloseCallback)(::Conn::Connection&);
	}

	namespace AddonLayer
	{
		typedef enum
		{
			CGI = 0,
			_FREE1,
			_FREE2,
			_FREE3,
			COUNT
		}	Type;
		typedef void* State;
	}

	struct BindInfo
	{
		AppLayer::Type		appLayer;
		Listen::backlog		backlog;
		Sock::addrFamily	family;
		Sock::type			socktype;
		Sock::protocol		proto;
		Sock::union_addr	addr;
		Sock::addrlen		addrlen;
	};
}

#endif
