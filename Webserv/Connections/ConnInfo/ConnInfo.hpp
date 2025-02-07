
#ifndef CONNECTIONSOCKET_HPP

# define CONNECTIONSOCKET_HPP

# include "../../Ws_Namespace.h"

struct ConnInfo
{
		ConnInfo(const Ws::Sock::fd sockfd, const Ws::BindInfo& info);
		ConnInfo();
		~ConnInfo();
		ConnInfo(const ConnInfo& copy);
		ConnInfo& operator=(const ConnInfo& assign);

		void				reset();

		Ws::Sock::fd		sockfd;
		Ws::BindInfo		bind;
};


#endif