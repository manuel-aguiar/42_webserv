

#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <cstdlib>

void closePipes(int pipefds[2])
{
	if (pipefds[0] != -1)
		close(pipefds[0]);
	if (pipefds[1] != -1)
		close(pipefds[1]);
	pipefds[0] = -1;
	pipefds[1] = -1;
}

void	closeFdSetMinusOne(int& fd)
{
	close(fd);
	fd = -1;
}


int main(void)
{
	int pipeParentToChild[2];
	int pipeChildToParent[2];
	int pid;
	char buffer[256];

	if (pipe(pipeParentToChild) == -1)
	{
		std::cerr << "pipe(): " << std::strerror(errno) << std::endl;
		return (EXIT_FAILURE);
	}
	if (pipe(pipeChildToParent) == -1)
	{
		std::cerr << "pipe(): " << std::strerror(errno) << std::endl;
		closePipes(pipeParentToChild);
		return (EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		std::cerr << "fork(): " << std::strerror(errno) << std::endl;
		closePipes(pipeParentToChild);
		closePipes(pipeChildToParent);
		return (EXIT_FAILURE);
	}

	// child
	if (!pid)
	{
		closeFdSetMinusOne(pipeParentToChild[1]);  //close write parent
		closeFdSetMinusOne(pipeChildToParent[0]);   //close read child
		read(pipeParentToChild[0], buffer, sizeof(buffer));		// block until it receives something
		std::cout   <<  "I'm child, parent told me: " << buffer << std::endl
					<< "		Now i'll send a message to dad" << std::endl;
		write(pipeChildToParent[1], "Hi Dad!!", 9);
	}

	// parent
	else
	{
		closeFdSetMinusOne(pipeParentToChild[0]);  //close read parent
		closeFdSetMinusOne(pipeChildToParent[1]);   //close write child
		std::cout << "		I'm Dad, sending message to son" << std::endl;
		write(pipeParentToChild[1], "Hi Son!!", 9);
		read(pipeChildToParent[0], buffer, sizeof(buffer));			 //block until it receives something
		std::cout <<  "I'm parent, child told me: " << buffer << std::endl;
		waitpid(pid, NULL, 0);
	}

	closePipes(pipeParentToChild);
	closePipes(pipeChildToParent);
}

// c++ -Wall -Wextra -Werror pipe.cpp -o pipe
// valgrind --track-fds=yes --trace-children=yes --leak-check=full ./pipe