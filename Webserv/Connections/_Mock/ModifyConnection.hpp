

#ifndef MOCKMODIFYCONNECTION_HPP

# define MOCKMODIFYCONNECTION_HPP

# include "../Connection/Connection.hpp"


class ModifyConnection : public Conn::Connection
{
	public:
		ModifyConnection(Events::Manager& eventManager, ServerContext& context);
		ModifyConnection(const ModifyConnection& copy);
		ModifyConnection& operator=(const ModifyConnection& assign);
		~ModifyConnection();


		void	setConnInfo(const ConnInfo& info);

};


#endif