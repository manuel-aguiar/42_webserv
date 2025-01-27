

#ifndef CONNECTIONINTERNALCONN_HPP

# define CONNECTIONINTERNALCONN_HPP

# include "../Connection/Connection.hpp"

class ListeningSocket;

class InternalConn : public Conn::Connection
{
	public:
		InternalConn(	Events::Manager& eventManager, 
						ImplManager& connManager, 
						ServerContext& serverContext);
		~InternalConn();

		InternalConn(const InternalConn& other);
		InternalConn& operator=(const InternalConn& other);

		int		accept(Ws::Sock::fd& listener);
		void 	ForceClose();

		Ws::Sock::addr&		accessAddr();
		Ws::Sock::addrlen&	accessAddrlen();
		Ws::Sock::fd		accessSocket();

	private:
		typedef enum
		{
			IDLE,
			ACTIVE
		}	State;

		State	m_state;
};

#endif