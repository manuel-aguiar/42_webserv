
#ifndef CONNECTIONSOCKET_HPP

# define CONNECTIONSOCKET_HPP

# include "../../Ws_Namespace.h"

class ConnInfo
{
	public:
		ConnInfo(const Ws::Sock::fd sockfd, const Ws::BindInfo& info);
		ConnInfo();
		~ConnInfo();
		ConnInfo(const ConnInfo& copy);
		ConnInfo& operator=(const ConnInfo& assign);

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