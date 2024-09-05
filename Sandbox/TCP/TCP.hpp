/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   TCP.hpp											:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/09/03 19:30:10 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/09/03 19:40:24 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

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
			_fd(socket(TransportLayer::addressFamily, 
						TransportLayer::socketType,
						TransportLayer::protocol))
		{
		}
		~Socket() 
		{
			close(_fd);
		}
	private:
		const int _fd;
};

#endif