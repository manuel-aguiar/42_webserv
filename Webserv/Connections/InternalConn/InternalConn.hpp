

#ifndef CONNINTERNALCONN_HPP

# define CONNINTERNALCONN_HPP

# include "../Connection/Connection.hpp"


class InternalConn : public Conn::Connection
{
	public:
		InternalConn(Events::Manager& eventManager, ServerContext& context, InternalManager& connManager);
		~InternalConn();


		void	prepareDispatch();
		void	forcedClose();
		void	reset();

		ConnInfo&	accessConnInfo();

	private:
	
		void	mf_callAppLayerForceClose();

};


#endif