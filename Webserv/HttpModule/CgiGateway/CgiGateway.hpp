

#ifndef HTTP_CGIGATEWAY_HPP

# define HTTP_CGIGATEWAY_HPP

# include "../../CgiModule/CgiModule.h"

namespace Http { class Request; }

namespace Http
{
	class CgiGateway
	{
		public:
			CgiGateway(Cgi::Request *request);
			~CgiGateway();
			void	launch();

			static void prepareRequest  (Http::CgiGateway& gateway, Http::Request& request);

			static void onSuccess		(Cgi::User user);
			static void onErrorStartup	(Cgi::User user);
			static void onErrorRuntime	(Cgi::User user);
			static void onErrorTimeOut	(Cgi::User user);

			static Cgi::IO::BytesCount 		onRead(Cgi::User user, int readFd);
			static Cgi::IO::BytesCount 		onWrite(Cgi::User user, int writeFd);


		private:
			Cgi::Request	*m_request;
	};
};



#endif