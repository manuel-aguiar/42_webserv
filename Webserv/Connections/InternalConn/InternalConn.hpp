

#ifndef CONNINTERNALCONN_HPP

# define CONNINTERNALCONN_HPP

# include "../Connection/Connection.hpp"


class InternalConn : public Conn::Connection
{
	public:
		InternalConn(InternalManager& connManager);
		~InternalConn();


		void	prepareDispatch();
		void	forcedClose();
		void	reset();

		ConnInfo&	accessSocket();

	private:
	
		void	mf_callAppLayerForceClose();

};


#endif