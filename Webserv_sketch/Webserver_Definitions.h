/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver_Definitions.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:56:52 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/11 17:00:06 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_DEFINITIONS_H

# define WEBSERVER_DEFINITIONS_H

# include <stdint.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/un.h>
# include <netdb.h>
# include <sys/epoll.h>
# include <cerrno>
# include <signal.h>
# include <cassert>

//own headers
# include "../Toolkit/Toolkit.h"

// definitions for everyone to use
typedef unsigned char       t_byte;
typedef uint16_t            t_port;
typedef int                 t_fd;
typedef int                 t_socket;

typedef struct sockaddr     t_sockaddr;
typedef struct sockaddr_in  t_sockaddr_in;
typedef struct sockaddr_in6 t_sockaddr_in6;
typedef struct sockaddr_un  t_sockaddr_un;
typedef socklen_t           t_socklen;
typedef struct addrinfo     t_addrinfo;

typedef struct epoll_event  t_epoll_event;

typedef struct sigaction    t_sigaction;
typedef sigset_t            t_sigset;

typedef union
{
    t_sockaddr         sockaddr;
    t_sockaddr_in      sockaddr_in;
    t_sockaddr_in6     sockaddr_in6;
    t_sockaddr_un      sockaddr_un;
}   u_sockaddr;

# define MAX_EPOLL_EVENTS 64



#endif
