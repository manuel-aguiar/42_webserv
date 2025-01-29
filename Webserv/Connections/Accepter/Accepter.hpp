

#ifndef CONNECTIONLISTENER_HPP

# define CONNECTIONLISTENER_HPP

class Socket;

class Accepter
{
	public:
		Accepter(int backlog);
		~Accepter();

		int						open(Socket& listen);
		void					close(Socket& listen);
		int						accept(const Socket& listen, Socket& accept);

	private:
		int		m_backlog;

		Accepter(const Accepter& copy);
		Accepter& operator=(const Accepter& assign);
};



#endif