

#ifndef CONNECTIONINTERNALCONN_HPP

# define CONNECTIONINTERNALCONN_HPP

# include "../Connection/Connection.hpp"

class InternalConn : public Conn::Connection
{
	public:
		InternalConn(	Events::Manager& eventManager, 
						ImplManager& connManager, 
						ServerContext& serverContext);
		~InternalConn();

		InternalConn(const InternalConn& other);
		InternalConn& operator=(const InternalConn& other);

		void ForceClose();

	private:
		typedef enum
		{
			IDLE,
			ACTIVE
		}	State;

		State	m_state;
};

#endif