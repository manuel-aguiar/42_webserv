

#ifndef SERVER_HPP

# define SERVER_HPP

/*
	Server is what is considered to be a website (directory tree).
	A server that "serves" content from multiple websites (directory trees), sees individual
	websites as "Servers"
*/

#include <set>

#include "../VirtualHost/VirtualHost.hpp"
#include "../../EventManager/EventManager.hpp"


template <
	typename ApplicationLayer,
	typename TransportLayer
>
class Server
{
	public:
		Server(const char* configFileName);
		~Server();
		Server(const Server& copy);
		Server& operator=(const Server& assign);
	
		void run();

	private:
		EventManager<TransportLayer> _manager;
		std::set<VirtualHost> hosts;


		Server();
		
};



#endif