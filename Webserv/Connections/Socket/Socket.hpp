
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

		void				reset();

		Ws::Sock::fd		getSockFd() const;
		const Ws::BindInfo&	getBindInfo() const;

		void				setSockFd(const Ws::Sock::fd sockfd);
		void				setBindInfo(const Ws::BindInfo& info);

		Ws::BindInfo&		modifyBindInfo();

	private:
		Ws::Sock::fd		m_sockfd;
		Ws::BindInfo		m_info;
};


#endif