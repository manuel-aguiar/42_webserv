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
	(cd ../../../Webserv_sketch/ThreadPool/ && make) && c++ -Wall -Wextra -Werror serverthread.cpp  -L../../../Webserv_sketch/ThreadPool/ -lthreadpool -lpthread -o serverthread

	valgrind --tool=helgrind --track-fds=yes --trace-children=yes ./serverthread 
*/

/*
	Using the threadpool, essentially each thread is a server, all listening sockets listen to the same port/address with SO_REUSEPORT

	if SO_REUSEPORT is not defined, well.. bind on the subsequent servers fails, one thread active and the remaining will be idle 


	To manage signals:
		each thread blocks SIGINT and SIGQUIT
			being process-level signals, the main thread will be the one receiving those
		each thread has an epoll
			each epoll has the read end of a pipe monitoring read operations
			(epoll here doesn't do much else apart from reporting connections from the listenning socket)

		global signal handler has a vector of pipes to write to
		signal_handler: write to these pipes
			epoll will notify when pipe is ready to read, if the fd is the correesponding signal pipe, exit the threadfunction (server)
		main thread does nothing else more than listening to signals and sleeping
			could use sigwait and resolve synchronously, as well

	ThreadWorkers don't listen to signals, main thread is the one who receives and manages them
	threads exit gracefully, no race conditions (noted so far)
	all memory is clean, threads joined, fds closed
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
# include <map>
# include <signal.h>
# include <sys/wait.h>
# include <unistd.h>
# include <sys/epoll.h>
# include <vector>
# include <fcntl.h>

# define EPOLL_MAXEVENTS 10

pthread_mutex_t threadlock;
void	lockWrite(const std::string& toWrite, std::ostream& stream);


class WebServerSignalHandler
{
	public:

		static int PipeRead(int numServer) {return _pipes[numServer].first;}
		static int PipeWrite(int numServer) {return _pipes[numServer].second;}

		static int getSignal() {return (WebServerSignalHandler::gm_signal);};

		static void		signal_handler(int sigNum)
		{
			if (sigNum == SIGINT || sigNum == SIGQUIT)
			{
				gm_signal = sigNum;
				for (size_t i = 0; i < _pipes.size(); ++i)
				{
					write(PipeWrite(i), "DUKE NUKEM", sizeof("DUKE NUKEM"));
				}
			}
		}

		static int prepare_signal(struct sigaction *ms, void (*handler)(int), int numServers)
		{
			int pipefd[2];

			ms->sa_flags = 0;
			ms->sa_handler = handler;
			sigemptyset(&(ms->sa_mask));
			sigaction(SIGINT, ms, NULL);
			sigaction(SIGQUIT, ms, NULL);
			for (int i = 0; i < numServers; ++i)
			{
				pipe(pipefd);
				_pipes.push_back(std::make_pair(pipefd[0], pipefd[1]));
			}
			return (1);
		}
		static void destroy_signal(struct sigaction *ms)
		{
			(void)ms;

			for (size_t i = 00; i < _pipes.size(); ++i)
			{
				close(PipeRead(i));
				close(PipeWrite(i));
			}
			_pipes.clear();
		}
	private:
		static	std::vector<std::pair<int, int> >	_pipes;
		static int 						gm_signal;
};

int		WebServerSignalHandler::gm_signal = 0;
std::vector<std::pair<int, int> > WebServerSignalHandler::_pipes;




class RemoteClient
{
	public:
		RemoteClient() : m_fd(-1), _event((struct epoll_event) {}) {}
		RemoteClient(const int fd) : m_fd(fd), _event((struct epoll_event) {}) {}
		RemoteClient(const int fd, const struct epoll_event event) : m_fd(fd), _event(event) {}
		~RemoteClient() {};
		RemoteClient(const RemoteClient& copy) : m_fd(copy.m_fd) {(void)copy;}
		RemoteClient& operator=(const RemoteClient& assign) {if (this == &assign) return (*this); m_fd = assign.m_fd; _event = assign._event; return (*this);}

		void setEvent(const struct epoll_event event) {_event = event;}
		struct epoll_event& getEvent() {return (_event);}
	private:
		int		 			m_fd;
		struct epoll_event	_event;
};

void closeconnections(std::map<int, RemoteClient>& connfds, int epollfd)
{
	std::map<int, RemoteClient>::iterator iter;
	for (iter = connfds.begin(); iter != connfds.end(); ++iter)
	{
		if (epoll_ctl(epollfd, EPOLL_CTL_DEL, iter->first, &(iter->second.getEvent())) == -1)
			lockWrite(std::string("epoll_ctl() potato: ") + std::strerror(errno), std::cerr);
		close(iter->first);
	}
	connfds.clear();
}

void closeSingleConn(int fd, std::map<int, RemoteClient>& connfds, int epollfd)
{
	RemoteClient& conn = connfds[fd];

	conn.setEvent((struct epoll_event){});
	if (epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &(conn.getEvent())) == -1)
	{
		lockWrite(std::string("epoll_ctl(): ") + std::strerror(errno), std::cerr);
		return ;
	}
	close(fd);
	connfds.erase(fd);
}

int ThreadServerFunc(int serverNumber)
{
	int				 		listener;
	struct sockaddr_in  	listAddress;
	struct sockaddr_in  	connAddress;
	socklen_t		   		connAddrLen;
	int				 		bytesRead;
	char					readBuff[256];
	int number = 1;

	std::map<int, RemoteClient>		connfds;

	int epollfd;
	int pipefdRead;

	struct epoll_event event;
	struct epoll_event wait[EPOLL_MAXEVENTS];


	epollfd = epoll_create1(EPOLL_CLOEXEC);
	pipefdRead = WebServerSignalHandler::PipeRead(serverNumber);

	event = (struct epoll_event){};	
	event.events = EPOLLIN;
	event.data.fd = pipefdRead;		//monitor pipe that will inform signal is received

	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, event.data.fd, &event) == -1)
	{
		lockWrite(std::string("epoll_ctl(): ") + std::strerror(errno), std::cerr);
		close(epollfd);
		return (EXIT_FAILURE);
	}



	std::memset(&listAddress, 0, sizeof(listAddress));
	listAddress.sin_family = AF_INET;
	listAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	listAddress.sin_port = htons(PORT);

	#ifdef SO_REUSEPORT
		int sockopt = SO_REUSEPORT | SO_REUSEADDR;
	#else
		int sockopt = SO_REUSEADDR
	#endif

	listener = socket(AF_INET6, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, IPPROTO_TCP);

	if (setsockopt(listener, SOL_SOCKET, sockopt, &number, sizeof(number)) == -1)
	{
		lockWrite(std::string("setsockopt(): ") + std::strerror(errno), std::cerr);
		errno = 0;
		close(listener);
		close(epollfd);
		return (1) ;			
	}

	if (bind(listener, (struct sockaddr*)&listAddress, sizeof(listAddress)) == -1)
	{
		lockWrite(std::string("bind(): ") + std::strerror(errno), std::cerr);
		close(listener);
		close(epollfd);
		return (EXIT_FAILURE);
	}
	
	if (listen(listener, MAX_CONNECTIONS) == -1)
	{
		lockWrite(std::string("listen(): ") + std::strerror(errno), std::cerr);
		close(listener);
		close(epollfd);
		return (EXIT_FAILURE);
	}

	event = (struct epoll_event){};	
	event.events = EPOLLIN;
	event.data.fd = listener;		//add listener socket to the poll

	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, event.data.fd, &event) == -1)
	{
		std::cerr << "epoll_ctl() " << std::strerror(errno) << std::endl;
		close(epollfd);
		close(listener);
		return (EXIT_FAILURE);
	}





	event = (struct epoll_event){};	
	event.events = EPOLLIN;
	event.data.fd = STDIN_FILENO;		//subscribe stdin to epoll to allow for reading commands, ONLY ONE EPOLL

	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, event.data.fd, &event) == -1)
	{
		std::cerr << "epoll_ctl() " << std::strerror(errno) << std::endl;
		close(epollfd);
		close(listener);
		return (EXIT_FAILURE);
	}
	


	
	lockWrite(std::string("  Server ") + std::to_string(serverNumber) + ": Waiting for connections....", std::cerr);

	connAddrLen = sizeof(connAddress);
	while(1)
	{

		int numWait = epoll_wait(epollfd, wait, EPOLL_MAXEVENTS, -1);	// BLOCK

		for (int i = 0; i < numWait; ++i)
		{
			if (wait[i].data.fd == pipefdRead)
			{
				lockWrite(std::string("  Server ") + std::to_string(serverNumber) + ": Signaled to exit", std::cerr);
				closeconnections(connfds, epollfd);
				close(listener);
				close(epollfd);
				return (EXIT_SUCCESS);
			}
			if (wait[i].data.fd == STDIN_FILENO && wait[i].events & EPOLLIN)
			{
				std::string buffer;
				
				std::memset(readBuff, 0, sizeof(readBuff));
				
				pthread_mutex_lock(&threadlock);
				bytesRead = read(wait[i].data.fd, readBuff, sizeof(readBuff) - 1);
				pthread_mutex_unlock(&threadlock);
				if (bytesRead != -1)
				{
					if (std::string(readBuff) == "STATUS\n")
						lockWrite(std::string("  Server ") + std::to_string(serverNumber) + ": all alive and kicking", std::cerr);
					else
						lockWrite(std::string("  Server ") + std::to_string(serverNumber) + ": unknown command", std::cerr);
				}
			}	
					
			else if (wait[i].data.fd == listener)
			{
				int newConn = accept(listener, (struct sockaddr*)&connAddress, &connAddrLen);
				if (newConn == -1)
				{
					lockWrite(std::string("accept(): ") + std::strerror(errno), std::cerr);
					closeconnections(connfds, epollfd);
					close(epollfd);
					close(listener);
					return (1);
				}
				event = (struct epoll_event){};	
				event.events = EPOLLIN | EPOLLOUT;
				event.data.fd = newConn;		//add listener socket to the poll

				connfds[newConn] = RemoteClient(newConn, event);

				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, event.data.fd, &event) == -1)
				{
					std::cerr << "epoll_ctl() " << std::strerror(errno) << std::endl;
					closeconnections(connfds, epollfd);
					close(epollfd);
					close(listener);
					return (EXIT_FAILURE);
				}
				
				lockWrite(std::string("  Server ") + std::to_string(serverNumber) + ": Connection received", std::cerr);
			}
			else
			{
				if (wait[i].events & EPOLLIN)
				{
					//lockWrite( std::string("\n  			Server ") + std::to_string(serverNumber) + " received message from client" ,std::cout);
					int totalRead = 0;
					while (true)
					{
						std::memset(readBuff, 0, sizeof(readBuff));
						bytesRead = read(wait[i].data.fd, readBuff, sizeof(readBuff) - 1);

						if (bytesRead == -1)
						{
							lockWrite(std::string("read(): ") + std::strerror(errno), std::cerr);
							close(epollfd);
							closeconnections(connfds, epollfd);
							close(listener);
							return EXIT_FAILURE;
						}
						else if (bytesRead == 0)
							break;
						else
						{
							totalRead += bytesRead;
							lockWrite(readBuff, std::cout);
							if (bytesRead < 255)
								lockWrite("", std::cout);
							if (readBuff[bytesRead] == '\0')
								break ;
						}
					}

					//lockWrite( std::string("\n  			Server ") + std::to_string(serverNumber) + " finished reading message, " + std::to_string(totalRead) + " bytes"  ,std::cout);
					if (wait[i].events & EPOLLOUT && !(wait[i].events & EPOLLHUP || wait[i].events & EPOLLERR))
					{
						//lockWrite( std::string("  			Server ") + std::to_string(serverNumber) + " can write to client" ,std::cout);
						int bytes = write(wait[i].data.fd, RESPONSE, std::strlen(RESPONSE) + 1);
						if (bytes == -1)
						{
							lockWrite(std::string("write(): ") + std::strerror(errno), std::cerr);
							close(epollfd);
							closeconnections(connfds, epollfd);
							close(listener);
							return (EXIT_FAILURE);
						}
						//lockWrite( std::string("  			Server ") + std::to_string(serverNumber) + " writing to client was successfull, " + std::to_string(bytes) + " bytes" ,std::cout);
					}
					//else
					//	lockWrite( std::string("  			Server ") + std::to_string(serverNumber) + " Client not ready to receive message" ,std::cout);
				}
				
				if (wait[i].events & EPOLLHUP || wait[i].events & EPOLLERR)
				{
					closeSingleConn(wait[i].data.fd, connfds, epollfd);
					lockWrite(std::string("  			Server ") + std::to_string(serverNumber) + "  Client closed the connection", std::cerr);
				}
			}
				

		}
	}
	closeconnections(connfds, epollfd);
	close(listener);
	close(epollfd);
	return (EXIT_SUCCESS);
}

void	lockWrite(const std::string& toWrite, std::ostream& stream)
{
	pthread_mutex_lock(&threadlock);
	stream << toWrite << std::endl;
	pthread_mutex_unlock(&threadlock);
}


int main(void)
{

	sigset_t threadSigSet;

	/* Disable SIGINT and SIGQUIT for the threadpool workers*/
	sigemptyset(&threadSigSet);
	sigaddset(&threadSigSet, SIGINT);
	sigaddset(&threadSigSet, SIGQUIT);

	pthread_sigmask(SIG_BLOCK , &threadSigSet, NULL);		// explicitely block sigint/quit for new threads		UNPROTECTED

	int numServers = 10;
	ThreadPool servers(numServers);

	/* Re-enable for the main thread*/
	pthread_sigmask(SIG_UNBLOCK, &threadSigSet, NULL);		// reestablish the oldmask set by the user		UNPROTECTED

	/* Signal Handler for the program*/
	struct sigaction signal = (struct sigaction){};
	WebServerSignalHandler::prepare_signal(&signal, WebServerSignalHandler::signal_handler, numServers);

	pthread_mutex_init(&threadlock, NULL);

	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

	for (int i = 0; i < numServers; ++i)
	{
		if (WebServerSignalHandler::getSignal())
			break ;
		servers.addTask(ThreadServerFunc, i);
	}

	while (!WebServerSignalHandler::getSignal())
		usleep(1000);

	servers.destroy(false);

	WebServerSignalHandler::destroy_signal(&signal);

	return (EXIT_SUCCESS);
}