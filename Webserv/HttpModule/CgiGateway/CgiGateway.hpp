

#ifndef HTTP_CGIGATEWAY_HPP

# define HTTP_CGIGATEWAY_HPP

# include "../../CgiModule/CgiModule.h"

namespace Http { class Connection; class Request; class Response;}

namespace Http
{
	class CgiGateway
	{
		public:
			CgiGateway(Cgi::Module& module, Http::Response& request);
			~CgiGateway();


			void	close();
			void	prepareRequest();

			// Cgi Callbacks
			static void onPrepareRequest  			(Http::CgiGateway& gateway);
			static void onSuccess					(Cgi::User user);
			static void onErrorStartup				(Cgi::User user);
			static void onErrorRuntime				(Cgi::User user);
			static void onErrorTimeOut				(Cgi::User user);
			static Cgi::IO::BytesCount 		onRead	(Cgi::User user, int readFd);
			static Cgi::IO::BytesCount 		onWrite	(Cgi::User user, int writeFd);


		private:



			
			Cgi::Module&		m_module;
			Cgi::Request*		m_cgiRequest;
			// Http
			Http::Response&		m_response;
			Http::Request&		m_request;
			Http::Connection&	m_connection;

			enum Parsing
			{
				HEADERS,
				BODY
			};
			
			Parsing 			m_state;
	};
}



#endif