

#ifndef CONNINTERNALCONN_HPP

# define CONNINTERNALCONN_HPP

# include "../Connection/Connection.hpp"


class InternalConn : public Conn::Connection
{
	public:
		InternalConn(ImplManager& connManager);
		~InternalConn();

		void	ForcedClose();
		void	reset();
		int 	beAccepted(Ws::Sock::fd listener, Ws::Sock::type type, Ws::Sock::protocol proto, Ws::AppLayer::Type appLayer);
	private:
		InternalConn(const InternalConn& copy);
		InternalConn& operator=(const InternalConn& assign);

};


#endif