/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   socketpair.cpp									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/23 12:13:18 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/23 14:00:39 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>

void closeSocketPair(int socketPair[2])
{
	close(socketPair[0]);
	close(socketPair[1]);
}

void	closeFdSetMinusOne(int& fd)
{
	close(fd);
	fd = -1;
}


int main(void)
{
	int socketPair[2];
	int pid;
	char buffer[256];

	// using UNIX domain protocol
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, socketPair) == -1)
	{
		std::cerr << "socketpair(): " << std::strerror(errno) << std::endl;
		return (EXIT_FAILURE);		
	}
	pid = fork();
	if (pid == -1)
	{
		std::cerr << "fork(): " << std::strerror(errno) << std::endl;
		closeSocketPair(socketPair);
		return (EXIT_FAILURE);
	}

	// child
	if (!pid)
	{
		recv(socketPair[0], buffer, sizeof(buffer), 0);		// block until it receives something
		std::cout   <<  "I'm child, parent told me: " << buffer << std::endl
					<< "		Now i'll send a message to dad" << std::endl;
		send(socketPair[0], "Hi Dad!!", 9, 0);
	}

	// parent
	else
	{
		std::cout << "		I'm Dad, sending message to son" << std::endl;
		send(socketPair[1], "Hi Son!!", 9, 0);
		recv(socketPair[1], buffer, sizeof(buffer), 0);			 //block until it receives something
		std::cout <<  "I'm parent, child told me: " << buffer << std::endl;
		waitpid(pid, NULL, 0);
	}

	closeSocketPair(socketPair);
}

// c++ -Wall -Wextra -Werror socketpair.cpp -o socketpair
// valgrind --track-fds=yes --trace-children=yes --leak-check=full ./socketpair