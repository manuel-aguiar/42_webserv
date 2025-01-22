// socket(), send(), recv(), accept(), connect(), bind(), listen()
# include <sys/socket.h>

//IPPROTO MACROS and more
# include <netinet/in.h>


// byte order converters
# include <arpa/inet.h>

# include <unistd.h>


# include <cstring>
# include <iostream>
# include <fstream>

# define PORT 8080
# define MAX_CONNECTIONS 10
# define WEBSITE_FOLDER "../Website/"
// # define RESPONSE "HTTP/1.0 200 OK\r\nContent-Length: 5\r\n\r\nHello" // response that shows up on the browser as plain text 


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

	std::memset(&listAddress, 0, sizeof(listAddress));
	listAddress.sin_family = AF_INET;
	listAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	listAddress.sin_port = htons(PORT);

	// Create socket   
	listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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

	// Load HTML file
	std::ifstream   file(std::string(WEBSITE_FOLDER) + "index.html");
	std::string	 htmlContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	std::string response = "HTTP/1.1 200 OK\r\nContent-Flags: text/html;\r\nset-cookie: bananas=potatoes;\r\ncharset=UTF-8\r\nContent-Length: " +
							std::to_string(htmlContent.size()) + "\r\n\r\n" + htmlContent;


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
		std::cout << "  Server: Client message:" << std::endl;
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
		std::cout << std::endl;
		std::cout << "  Server: End of client message." << std::endl;
		int byteswritten = write(connection, response.c_str(), response.length());
		if (byteswritten  == -1)
		{
			std::cerr << "write(): " << std::string (std::strerror(errno)) << std::endl;
			close(connection);
			close(listener);
			return (EXIT_FAILURE);
		}
		else
			std::cout << "write success: " << byteswritten << "\n\n" << response << "\n\n" << std::endl;
		close(connection);
	}
	

	close(listener);
	return (EXIT_SUCCESS);
}