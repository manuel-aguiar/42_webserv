/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   main.cpp										   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/09/02 07:54:47 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/09/02 07:59:27 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

/*
	https://jameshfisher.com/2018/02/03/what-does-getaddrinfo-do/
*/

//c headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
// c++ stl headers
#include <iostream>
#include <vector>
#include <cstring>
#include <string>

#define SOCKET_BACKLOG 10

void	printIPaddress(struct addrinfo* info)
{
	char ipstr[INET6_ADDRSTRLEN];
	void *addr;
	std::string ipversion;

	switch (info->ai_family)
	{
		case AF_INET:
		{
			struct sockaddr_in* ipv4 = (struct sockaddr_in *)info->ai_addr;
			addr = &(ipv4->sin_addr);
			ipversion = "IPV4";
			break ;
		}
		case AF_INET6:
		{
			struct sockaddr_in6* ipv6 = (struct sockaddr_in6 *)info->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipversion = "IPV6";
			break ;
		}
		default:
		{
			std::cerr << "unrecognized ai_family" << std::endl;
			return ;
		}
	}
	inet_ntop(info->ai_family, addr, ipstr, sizeof (ipstr));
	std::cout << ipversion << ": " << ipstr << std::endl;
}

int main(void)
{
	struct addrinfo hints;
	struct addrinfo* res;
	struct addrinfo* cur;

	

	std::vector<int>	listeningSockets;

	int status;
	int yes = 1;

	hints = (struct addrinfo){};
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

/*
	hints.ai_socktype = SOCK_DGRAM;

	listen(): Operation not supported
	UDP is a connectionless protocol, so a server cannot listen to connections via UDP
	because there are... no connections

	

*/
	

	status = getaddrinfo(NULL, "8080", &hints, &res);
	if (status != 0)
	{
		std::cerr << "getaddrinfo(): " + std::string(gai_strerror(status)) << std::endl;
		return (1);
	}

	for(cur = res; cur != NULL; cur = cur->ai_next)
	{
		printIPaddress(cur);

		int sockfd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);
		if (sockfd == -1)
		{
			std::cerr << "socket(): " << std::strerror(errno) << std::endl;
			continue ;
		}
		(void)yes;
		/*
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		{
			std::cerr << "setsockopt(): " << std::strerror(errno) << std::endl;
			errno = 0;
			close(sockfd);
			continue ;			
		}
		*/

		if (bind(sockfd, cur->ai_addr, cur->ai_addrlen) == -1)
		{
			std::cerr << "bind(): " << std::strerror(errno) << std::endl;
			errno = 0;
			close(sockfd);
			continue ;			
		}
		if (listen(sockfd, SOCKET_BACKLOG) == -1)
		{
			std::cerr << "listen(): " << std::strerror(errno) << std::endl;
			errno = 0;
			close(sockfd);
			continue;
		}
		listeningSockets.push_back(sockfd);		
	}
	freeaddrinfo(res);
	if (listeningSockets.size() == 0)
	{
		std::cerr << "No sockets were bound" << std::endl;
		return (1);
	}
	else
		std::cout << listeningSockets.size() << " sockets were bound" << std::endl;
	for (unsigned int i = 0; i < listeningSockets.size(); ++i)
	{
		close(listeningSockets[i]);
	}

	return (0);

	/*
		If the program is fire multiple times, sockets still bind because no full connection cycle took place
	*/
}





/*
int main(void) {
  struct addrinfo* addr;
  int result = getaddrinfo("google.com", NULL, NULL, &addr);
  if (result != 0) {
	std::cerr << "Error from getaddrinfo: " + std::string(gai_strerror(result)) << std::endl;
	return (EXIT_FAILURE);
  }
  struct sockaddr_in* internet_addr = (struct sockaddr_in*) addr->ai_addr;
  printf("google.com is at: %s\n", inet_ntoa(internet_addr->sin_addr));
  return (EXIT_SUCCESS);
}
*/


