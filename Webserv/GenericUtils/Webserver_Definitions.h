

#ifndef WEBSERVER_DEFINITIONS_H
# define WEBSERVER_DEFINITIONS_H

# include <stdint.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/un.h>
# include <netdb.h>

# ifdef __linux__
#  include <sys/epoll.h>
# endif

# ifdef __APPLE__
#  include <sys/event.h>
# endif

# include <cerrno>
# include <signal.h>
# include <cassert>
# include <string>

// definitions for everyone to use
typedef unsigned char								t_byte;
typedef uint16_t									t_port;
typedef int											t_fd;
typedef pid_t										t_pid;
typedef int											t_socket;

typedef std::string									t_port_str;
typedef std::string									t_ip_str;
typedef std::string									t_path;
typedef std::string									t_server_name;
typedef std::pair<t_ip_str, t_port_str>				t_listeners;

typedef struct sockaddr								t_sockaddr;
typedef struct sockaddr_in							t_sockaddr_in;
typedef struct sockaddr_in6							t_sockaddr_in6;
typedef struct sockaddr_un							t_sockaddr_un;
typedef socklen_t									t_socklen;
typedef struct addrinfo								t_addrinfo;

typedef struct epoll_event							t_epoll_event;

typedef struct sigaction							t_sigaction;
typedef sigset_t									t_sigset;

# define MAX_EPOLL_EVENTS 64

namespace Conn { class Connection;}

namespace Ws
{
	typedef int			fd;
	enum { FD_NONE = -1 };
	typedef pid_t		pid;

	namespace Sock
	{
		typedef int					fd;
		typedef socklen_t			addrlen;
		typedef int					addrFamily;
		typedef int					type;
		typedef int					protocol;
		typedef union
		{
			t_sockaddr         sockaddr;
			t_sockaddr_in      sockaddr_in;
			t_sockaddr_in6     sockaddr_in6;
			t_sockaddr_un      sockaddr_un;
		}   addr;
	}

	namespace AppLayer
	{
		typedef enum
		{
			HTTP = 0,
			COUNT
		}	Type;
		typedef void* Module;
		typedef void* Conn;
		typedef void (*Init)(Conn::Connection&);
		typedef void (*CloseCallback)(Conn::Connection&);
	}

	namespace AddonLayer
	{
		typedef enum
		{
			CGI = 0,
			COUNT
		}	Type;
		typedef void* State;
	}

	typedef struct s_BindInfo
	{
		AppLayer::Type		appLayer;
		Sock::addrFamily	family;
		Sock::type			socktype;
		Sock::protocol		proto;
		Sock::addr			addr;
		Sock::addrlen		addrlen;
	}	BindInfo;
}

#endif
