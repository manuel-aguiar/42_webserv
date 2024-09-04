/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   main.cpp										   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/29 10:32:00 by manuel			#+#	#+#			 */
/*   Updated: 2024/09/03 11:48:05 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include <sys/epoll.h>

/*
epoll (epoll_create,
epoll_ctl, epoll_wait), 
*/


/*


	This time we will open a pipe where child writes to parent

	Child will execve a command with args passed via main;
	dup2 write end to stdout;

	parent will use epoll to monitor traffic, marking the read end of the pipe
	to fire an event when it is ready to be read from

	epoll don0t hang, (short timeout, could be zero be we set to 1)

	once the pipe is ready to read, parent writes the message received from child.

	compile: c++ -Wall -Wextra -Werror main.cpp
	example usage: ./a.out /bin/cat main.cpp

	valgrind --leak-check=full --track-fds=yes --trace-children=yes ./a.out /bin/cat main.cpp



	Notes:
		using epoll_create1, we can set the close on exec flag directly without an extra call to fcntl
			-> child doesn't leak fd on execve
		epoll always tracks EPOLLHUP and EPOLLERR
			errors and connection hangup (either socket or pipe) will always pop, those events don't have to be subscribed

		
*/


#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <fcntl.h>


#include <iostream>
#include <cstring>



#define MAX_EVENTS 10


int main(int ac, char **av, char **env)
{
	if (ac == 1)
		return (EXIT_FAILURE);
	int epollfd;
	int pipefd[2];
	int pid;
	int status;

	
	char readbuf[4];
	int bytesRead;
	std::string res;
	
	struct epoll_event event;
	struct epoll_event wait[MAX_EVENTS];
	int numWait;

	//create epoll instance
	epollfd = epoll_create1(EPOLL_CLOEXEC);
	/*
		or:

		epollfd = epoll_create(1);
		if (fcntl(epollfd, F_SETFD, FD_CLOEXEC) == -1)
		{
			std::cerr << "fcntl() " << std::strerror(errno) << std::endl;
			close(epollfd);
			return (EXIT_FAILURE);
		}
	*/

	if (epollfd == -1)
	{
		std::cerr << "epoll_create1() " << std::strerror(errno) << std::endl;
		return (EXIT_FAILURE);
	}


	//open pipe	
	if (pipe(pipefd) == -1)
	{
		std::cerr << "epoll_create1() " << std::strerror(errno) << std::endl;
		close(epollfd);
		return (EXIT_FAILURE);
	}

	//subscribe the piperead fd to the epoll
	event = (struct epoll_event){};	 //required, event.data is a union, if you use int (4bytes) you have another 4bytes of garbage (union with void *ptr);
	event.events = EPOLLIN;
	event.data.fd = pipefd[0];

	//epoll wait will hang forever with -1 timeout and no fds to monitor
	//epoll_wait(epollfd, wait, MAX_EVENTS, -1);


	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, pipefd[0], &event) == -1)
	{
		std::cerr << "epoll_ctl() " << std::strerror(errno) << std::endl;
		close(epollfd);
		close(pipefd[0]);
		close(pipefd[1]);
		return (EXIT_FAILURE);
	}


	pid = fork();



	//child
	if (!pid)
	{

		close(pipefd[0]);							   //child closes pipe-read
		//close(pipefd[1]);
		//exit(1);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)	   // redirect write-end of the pipe to stdout;
		{
			std::cerr << "dup2() " << std::strerror(errno) << std::endl;
			close(epollfd);
			close(pipefd[1]);
			return (EXIT_FAILURE);
		}

		close(pipefd[1]);


		// some async BS
		write(STDOUT_FILENO, "HEYHEYHEY\n", sizeof("HEYHEYHEY\n"));

		sleep(2);

		write(STDOUT_FILENO, "YOYOYO\n", sizeof("YOYOYO\n"));

		sleep(2);
		
		//exec the command;
		if (execve(av[1], &av[1], env) == -1)
		{
			std::cerr << "execve() " << std::strerror(errno) << std::endl;
			close(epollfd);
			close(pipefd[1]);
			return (EXIT_FAILURE);
		}
	}

	//parent
	else
	{
		close(pipefd[1]); /// close pipe-write
		bool exitLoop = false;

		// set piperead to non-blocking, if there is nothing to read without hungup, just move along
		if (fcntl(pipefd[0], F_SETFL, O_NONBLOCK) == -1)
			std::cerr << "fcntl failed" << std::endl;

		
		std::cout << "Parent is going to wait... " << std::endl;
		while (!exitLoop)
		{
			
			std::memset(wait, 0, sizeof(wait));
			numWait = epoll_wait(epollfd, wait, MAX_EVENTS, 1);	//timeout eent 1, don't hang
			if (numWait == -1)
			{
				std::cerr << "epoll_wait() " << std::strerror(errno) << std::endl;
				close(epollfd);
				close(pipefd[0]);
				return (EXIT_FAILURE);
			}
			for (int i = 0; i < numWait; ++i)
			{
				if (wait[i].data.fd == pipefd[0])	   //i don't care, i'm only listening to one
				{   
					bytesRead = read(pipefd[0], readbuf, sizeof(readbuf) - 1);

					if ((wait[i].events & EPOLLHUP) && bytesRead == 0)
					{
						// pipe was closed (EPOLLHUP) and it is empty, unsubscribe from epoll
						if (epoll_ctl(epollfd, EPOLL_CTL_DEL, wait[i].data.fd, NULL) == -1)
							std::cerr << "epoll_ctl() " << std::strerror(errno) << std::endl;
						exitLoop = true;
					}
					
					readbuf[bytesRead] = '\0';
					res += readbuf;
					std::cout << "		  read " << bytesRead << std::endl;

				}
			}
		}

		
		std::cout << "\n\nfiles waiting " << epoll_wait(epollfd, wait, MAX_EVENTS, 1) << std::endl;

		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			std::cout << "Child process finished with exit status: " << WEXITSTATUS(status) << std::endl;

		std::cout << res << std::endl;
		
		close(epollfd);
		close(pipefd[0]);   //parent closes piperead;
	}

	return (EXIT_SUCCESS);

}


//pipe open, nothing to read, read returns -1;
//pipe closed, nothing to read, read returns 0;