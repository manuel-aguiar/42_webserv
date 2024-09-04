/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   server.cpp										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/23 08:42:27 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/30 09:59:55 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */


/*
	Using SO_REUSEPORT to allow multiple listening sockets on the same address and port
	Having children processes spawn their own socket on the same port

	kernel distributes traffic evenly

	use ./client to contact the server and check which child responds.

	The subject says it is forbidden to execve another server.......
	But doesn't state anything about forking the program to have multiple epolls
	and listening sockets, one case use binary semaphores to communicate

	Concurrency problems though.....



	We could also have 2 threadpools
		One pool for listening sockets, each with its own eventmanager, managing their own

		Second to subscribe tasks
		Easier to coordinate with mutexes in case there are changes to the state of the website




	Signals to force chuildren out. parent ignores signals and just waits

*/

// socket(), send(), recv(), accept(), connect(), bind(), listen()
# include <sys/socket.h>

//IPPROTO MACROS and more
# include <netinet/in.h>


// byte order converters
# include <arpa/inet.h>

# include <unistd.h>


# include <cstring>
# include <iostream>

# include "../../../Webserv_sketch/ThreadPool/include/Concrete/ThreadPool.hpp"

# define PORT 8080
# define MAX_CONNECTIONS 10
# define RESPONSE "Hello Client!"


/*
struct sockaddr_in {
	short			sin_family;   // Address family (AF_INET)
	unsigned short   sin_port;	 // Port number (in network byte order)
	struct in_addr   sin_addr;	 // Internet address (struct in_addr)
};

multithreading and signals

	signal handlers cannot contain mutex lock:

	signal -> thread 1 received, calls signal_handler, locks ... 
	signal_handler interrupted by signal		
			->thread 1 received, calls signal_handler, locks ... deadlock by itself

	for instance, the signal handler can safely call sem_post but not the sem_wait (in the same spirit);


*/

# include <vector>
# include <signal.h>
# include <sys/wait.h>

int g_signal;

void	signal_handler(int signum)
{
	if (signum == SIGINT)
		g_signal = SIGINT;
	if (signum == SIGQUIT)
		g_signal = SIGQUIT;
}

int	ignore_signal(struct sigaction *ms)
{
	ms->sa_flags = SA_RESTART;			//restart waitpid, keep waiting for children
	ms->sa_handler = SIG_IGN;
	sigemptyset(&(ms->sa_mask));
	sigaction(SIGINT, ms, NULL);
	sigaction(SIGQUIT, ms, NULL);
	return (1);
}

int prepare_signal(struct sigaction *ms, void (*handler)(int))
{
	ms->sa_flags = 0;					// stuck in accept call, don't restart, move on and if there was a signal -> it is part of the plan
	ms->sa_handler = handler;
	sigemptyset(&(ms->sa_mask));
	sigaction(SIGINT, ms, NULL);
	sigaction(SIGQUIT, ms, NULL);
	return (1);
}


/*
	c++ -Wall -Wextra -Werror serverchildren.cpp -o serverchild
*/

int checkglobal()
{
	int signal;
	static pthread_mutex_t* mutex;

	if (!mutex)
	{
		mutex = new pthread_mutex_t;
		pthread_mutex_init(mutex, NULL);
	}
	pthread_mutex_lock(mutex);
	signal = g_signal;
	pthread_mutex_unlock(mutex);
}


int main()
{
	int				 listener;
	int	 			connection;
	struct sockaddr_in  listAddress;
	struct sockaddr_in  connAddress;
	socklen_t		   connAddrLen;
	int				 bytesRead;
	char				readBuff[256];
	int number = 1;

	struct sigaction signal = (struct sigaction) {};

	std::memset(&listAddress, 0, sizeof(listAddress));
	listAddress.sin_family = AF_INET;
	listAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	listAddress.sin_port = htons(PORT);
	
	#ifdef SO_REUSEPORT
		int sockopt = SO_REUSEPORT | SO_REUSEADDR;
	#else
		int sockopt = SO_REUSEADDR
	#endif



	int serverCount = 10;
	pid_t pid;
	std::vector<pid_t> servers;

	ignore_signal(&signal);

	for (int i = 0; i < serverCount; ++i)
	{
		pid = fork();

		//parent;
		if (pid)
			servers.push_back(pid);

		//child, stuck on loop and always returning (exiting)
		else
		{
			prepare_signal(&signal, signal_handler);


			listener = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_TCP);

			if (setsockopt(listener, SOL_SOCKET, sockopt, &number, sizeof(number)) == -1)
			{
				std::cerr << "setsockopt(): " << std::strerror(errno) << std::endl;
				errno = 0;
				close(listener);
				return (1) ;			
			}

			if (bind(listener, (struct sockaddr*)&listAddress, sizeof(listAddress)) == -1)
			{
				std::cerr << "bind(): " << std::string (std::strerror(errno)) << std::endl;
				close(listener);
				return (EXIT_FAILURE);
			}
			
			if (listen(listener, MAX_CONNECTIONS) == -1)
			{
				std::cerr << "listen(): " << std::string (std::strerror(errno)) << std::endl;
				close(listener);
				return (EXIT_FAILURE);
			}


			connAddrLen = sizeof(connAddress);
			while(!g_signal)
			{
				std::cout << "  Server " << i << ": Waiting for connections...." << std::endl;
				connection = accept(listener, (struct sockaddr*)&connAddress, &connAddrLen);
				if (connection == -1)
				{
					if (!g_signal)
						std::cerr << "accept(): " << std::string (std::strerror(errno)) << std::endl;
					close(listener);
					return (g_signal != 0);
				}
				std::cout << "  Server " << i << " Connection received" << std::endl;
				while (true)
				{
					std::memset(readBuff, 0, sizeof(readBuff));
					bytesRead = read(connection, readBuff, sizeof(readBuff) - 1);

					if (bytesRead == -1)
					{
						std::cerr << "read(): " << std::strerror(errno) << std::endl;
						close(connection);
						close(listener);
						return EXIT_FAILURE;
					}
					else if (bytesRead == 0)
					{
						std::cout << "  Server " << i << "Client closed the connection" << std::endl;
						break;
					}
					else
					{
						std::cout << readBuff;
						if (readBuff[bytesRead] == '\0')
							break ;
					}
				}
				if (write(connection, RESPONSE, std::strlen(RESPONSE) + 1) == -1)
				{
					std::cerr << "write(): " << std::string (std::strerror(errno)) << std::endl;
					close(connection);
					close(listener);
					return (EXIT_FAILURE);
				}
				std::cout << std::endl;
				close(connection);
			}
			

			close(listener);
			return (EXIT_SUCCESS);
		}
	}
	for (int i = 0; i < serverCount; ++i)
	{
		waitpid(-1, NULL, 0);
	}
	return (EXIT_SUCCESS);
}