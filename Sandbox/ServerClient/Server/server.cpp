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

// socket(), send(), recv(), accept(), connect(), bind(), listen()
# include <sys/socket.h>

//IPPROTO MACROS and more
# include <netinet/in.h>


// byte order converters
# include <arpa/inet.h>

# include <unistd.h>


# include <cstring>
# include <iostream>

# define PORT 8080
# define MAX_CONNECTIONS 10
# define RESPONSE "Hello Client!"

/*
struct sockaddr_in {
	short			sin_family;   // Address family (AF_INET)
	unsigned short   sin_port;	 // Port number (in network byte order)
	struct in_addr   sin_addr;	 // Internet address (struct in_addr)
};
*/

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

	std::memset(&listAddress, 0, sizeof(listAddress));
	listAddress.sin_family = AF_INET;
	listAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	listAddress.sin_port = htons(PORT);
	
	listener = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_TCP);

	/*
		https://lwn.net/Articles/542629/
		https://stackoverflow.com/questions/14388706/how-do-so-reuseaddr-and-so-reuseport-differ

		SO_REUSEPORT (NOT PORTABLE, linux 3.9)
			-> allow for multiple sockets listening on the same port
			-> rogue programs listening as well trying to get packets.....?

		SO_REUSEADDR
			-> when closing the socket, tell the kernel to not enter TIME_WAIT state for the socket
			and forget all incoming packets that might be travelling
			-> port is available to bind immediately by another socket

		"
		Linux >= 3.9
			Linux 3.9 added the option SO_REUSEPORT to Linux as well. This option 
			behaves exactly like the option in BSD and allows binding to exactly 
			the same address and port number as long as all sockets have this option 
			set prior to binding them.

			Yet, there are still two differences to SO_REUSEPORT on other systems:

			To prevent "port hijacking", there is one special limitation: All sockets 
			that want to share the same address and port combination must belong to 
			processes that share the same effective user ID! So one user cannot "steal" 
			ports of another user. This is some special magic to somewhat compensate for 
			the missing SO_EXCLBIND/SO_EXCLUSIVEADDRUSE flags.

			Additionally the kernel performs some "special magic" for SO_REUSEPORT sockets 
			that isn't found in other operating systems: For UDP sockets, it tries to distribute 
			datagrams evenly, for TCP listening sockets, it tries to distribute incoming 
			connect requests (those accepted by calling accept()) evenly across all the 
			sockets that share the same address and port combination. Thus an application 
			can easily open the same port in multiple child processes and then use 
			SO_REUSEPORT to get a very inexpensive load balancing.
		
		"

		#ifdef SO_REUSEPORT
			int sockopt = SO_REUSEPORT | SO_REUSEADDR
		#else
			int sockopt = SO_REUSEADDR
	*/

	#ifdef SO_REUSEPORT
		int sockopt = SO_REUSEPORT | SO_REUSEADDR;
	#else
		int sockopt = SO_REUSEADDR
	#endif

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
	while(true)
	{
		std::cout << "  Server: Waiting for connections...." << std::endl;
		connection = accept(listener, (struct sockaddr*)&connAddress, &connAddrLen);
		if (connection == -1)
		{
			std::cerr << "accept(): " << std::string (std::strerror(errno)) << std::endl;
			close(listener);
			return (EXIT_FAILURE);
		}
		std::cout << "  Server: Connection received" << std::endl;
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
				std::cout << "Client closed the connection" << std::endl;
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