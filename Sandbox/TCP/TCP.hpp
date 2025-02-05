
#ifndef TCP_HPP

# define TCP_HPP

# include <sys/socket.h>
# include <netinet/ip.h>

class TCP
{
	public:
		static const int	addressFamily = AF_INET;
		static const int	socketType = SOCK_STREAM;
		static const int	protocol = IPPROTO_TCP;
};


# include <unistd.h>

template <
	typename TransportLayer
> class Socket
{

	public:
		Socket() :
			m_fd(socket(TransportLayer::addressFamily, 
						TransportLayer::socketType,
						TransportLayer::protocol))
		{
		}
		~Socket() 
		{
			close(m_fd);
		}
	private:
		const int m_fd;
};

#endif