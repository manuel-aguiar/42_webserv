

#ifndef HTTPCGIHANDLERS_HPP

# define HTTPCGIHANDLERS_HPP

# include "../../CgiModule/CgiNamespace.h"
# include "../../Ws_Namespace.h"

namespace Http
{
	struct CgiHandlers
	{
		static void                 onSuccess(Cgi::User user);
		static void                 onError(Cgi::User user);

		static Cgi::IO::State 		onRead(Cgi::User user, const Ws::fd readFd);
		static Cgi::IO::State 		onWrite(Cgi::User user, const Ws::fd writeFd);
		static Cgi::IO::State		onReceiveHeaders(Cgi::User user, const Cgi::HeaderData& headers);		
	};
}

#endif