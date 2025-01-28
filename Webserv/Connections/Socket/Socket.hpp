
#ifndef CONNECTIONSOCKET_HPP

# define CONNECTIONSOCKET_HPP

# include "../../GenericUtils/Webserver_Definitions.h"

class Socket
{
	public:
		Socket(const Ws::Sock::fd sockfd, const Ws::BindInfo& info);
		Socket();
		~Socket();
		Socket(const Socket& copy);
		Socket& operator=(const Socket& assign);

		Ws::Sock::fd		getSockFd();
		const Ws::BindInfo&	getBindInfo();

		void				setSockFd(const Ws::Sock::fd sockfd);
		void				setBindInfo(const Ws::BindInfo& info);

		Ws::BindInfo&		modifyBindInfo();
		
	private:
		Ws::Sock::fd		m_sockfd;
		Ws::BindInfo		m_info;
};


#endif