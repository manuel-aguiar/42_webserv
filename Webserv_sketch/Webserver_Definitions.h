/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver_Definitions.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:56:52 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/27 15:16:18 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_DEFINITIONS_H

# define WEBSERVER_DEFINITIONS_H

#include <stdint.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <sys/epoll.h>

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


//own headers
# include "MemoryPool/MemoryPool.h"


#endif