

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

typedef void*										t_ptr_ProtoConnection;
typedef void*										t_ptr_ProtoModule;


typedef union
{
	t_sockaddr         sockaddr;
	t_sockaddr_in      sockaddr_in;
	t_sockaddr_in6     sockaddr_in6;
	t_sockaddr_un      sockaddr_un;
}   u_sockaddr;

typedef struct s_BindAddress
{
	int					addrFamily;
	int					socktype;
	int					protocol;
	u_sockaddr			address;
	t_socklen			addrlen;
}	BindAddress;

# define MAX_EPOLL_EVENTS 64

namespace Ws
{
	typedef int			fd;
	typedef pid_t		pid;
}

#endif
