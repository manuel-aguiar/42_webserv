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

// c++ stl headers
#include <iostream>

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

