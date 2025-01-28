

#ifndef CONNINTERNALCONN_HPP

# define CONNINTERNALCONN_HPP

# include "../Connection/Connection.hpp"


class InternalConn : public Conn::Connection
{
	public:
		InternalConn(InternalManager& connManager);
		~InternalConn();

		void	ForcedClose();
		void	reset();

	private:
	
		void	mf_callAppLayerForceClose();

};


#endif