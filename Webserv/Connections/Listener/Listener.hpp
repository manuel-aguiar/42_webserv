

#ifndef CONNECTIONLISTENER_HPP

# define CONNECTIONLISTENER_HPP

# include "../Socket/Socket.hpp"

class Listener
{
	public:
		Listener(int backlog, const Ws::BindInfo& info);
		~Listener();

		int						open();
		void					close();
		int						accept(Socket& fill);

	private:
		Socket	m_socket;
		int		m_backlog;

		Listener(const Listener& copy);
		Listener& operator=(const Listener& assign);
};



#endif