

#ifndef CONNECTIONLISTENER_HPP

# define CONNECTIONLISTENER_HPP

# include "../Socket/Socket.hpp"

class Listener
{
	public:
		Listener(int backlog);
		~Listener();

		int						open(Socket& listen);
		void					close(Socket& listen);
		int						accept(const Socket& listen, Socket& accept);

	private:
		int		m_backlog;

		Listener(const Listener& copy);
		Listener& operator=(const Listener& assign);
};



#endif