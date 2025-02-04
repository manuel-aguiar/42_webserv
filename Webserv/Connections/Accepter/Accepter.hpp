

#ifndef CONNECTIONACCEPTER_HPP

# define CONNECTIONACCEPTER_HPP

class Socket;

namespace Ws { namespace Listen { typedef int backlog; } }

class Accepter
{
	public:
		Accepter(Ws::Listen::backlog backlog);
		~Accepter();

		int						open(Socket& listen);
		void					close(Socket& listen);
		int						accept(const Socket& listen, Socket& accept);

	private:
		Ws::Listen::backlog		m_backlog;

		Accepter(const Accepter& copy);
		Accepter& operator=(const Accepter& assign);
};



#endif