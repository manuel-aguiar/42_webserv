

#ifndef HTTP_CGIGATEWAY_HPP

# define HTTP_CGIGATEWAY_HPP

# include "../../CgiModule/CgiModule.h"
# include "../../Ws_Namespace.h"

namespace Http { class Connection; class Request; class Response;}

namespace Http
{
	class CgiGateway
	{
		public:
			CgiGateway(Cgi::Module& module, Http::Response& request);
			~CgiGateway();

			// to be called with interactions from client
			void	read();


			
			void	close();
			void	prepareRequest();
			
			void	storeReadAvailable(const Ws::fd& readFd);

			// to be called via callback from the CgiModule
			Cgi::IO::BytesCount	write(Ws::fd writeFd);

			// Cgi Callbacks
			static void onPrepareRequest  			(Http::CgiGateway& gateway);
			static void onSuccess					(Cgi::User user);
			static void onErrorStartup				(Cgi::User user);
			static void onErrorRuntime				(Cgi::User user);
			static void onErrorTimeOut				(Cgi::User user);
			static Cgi::IO::BytesCount 		onRead	(Cgi::User user, Ws::fd readFd);
			static Cgi::IO::BytesCount 		onWrite	(Cgi::User user, Ws::fd writeFd);


		private:

			Cgi::Module&		m_module;
			Cgi::Request*		m_cgiRequest;
			// Http
			Http::Response&		m_response;
			Http::Request&		m_request;
			Http::Connection&	m_connection;

			enum Parsing
			{
				NONE,
				HEADERS,
				CHUNKED,
				STREAM
			};

			Parsing 			m_state;
			Ws::fd				m_readFd;
			bool				m_readAvailable;
			size_t				m_msgBodyOffset;
	};
}



#endif