

#ifndef CONNECTIONLISTENER_HPP

# define CONNECTIONLISTENER_HPP

# include "../../Ws_Namespace.h"

class ConnInfo;
namespace Ws { namespace Listen { typedef int backlog; } }

class Listener
{
	public:
		Listener(const Ws::BindInfo& socketInfo);
		~Listener();
		Listener(const Listener& copy);
		Listener& operator=(const Listener& assign);

		int						open();
		void					close();
		int						accept(ConnInfo& accept);

		Ws::fd					getSockFd();
		const Ws::BindInfo&		getBindInfo();
	private:
		Ws::Sock::fd			m_sockfd;
		Ws::BindInfo			m_bindInfo;

};



#endif