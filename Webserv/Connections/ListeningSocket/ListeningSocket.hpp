

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP


// Project Headers
# include "../../GenericUtils/Webserver_Definitions.h"

//forward declarations
class InternalConn;
class Globals;
class ServerContext;

class ListeningSocket
{
	public:
		ListeningSocket(const int backlog, const Ws::BindInfo& info, Globals& globals);
		~ListeningSocket();

		// methods
		int                         open();
		int							accept(Conn::BaseConnection& connection);
		void                        close();

	private:
		Ws::Sock::fd				m_sockfd;
		int							m_backlog;

		const Ws::BindInfo&			m_info;
		Globals&					m_globals;

		ListeningSocket(const ListeningSocket& copy);
		ListeningSocket& operator=(const ListeningSocket& assign);

};


#endif
