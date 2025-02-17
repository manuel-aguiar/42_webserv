#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "./HttpDefinitions.hpp"

// move to relevant place
#define DEFAULT_ERROR_PAGES_PATH "Webserv/StaticAssets/ErrorPages/error_"

namespace Http {
	class Response {
		public:
			Response(Http::Connection& myConnection, Http::Request& myRequest, ServerContext& context);
			
			std::string	&Http::Response::getMessage(size_t statusCode);
		private:
			int generateResponse(int statusCode);

			Http::Connection&		m_myConnection;
			Http::Request&			m_myRequest;
			ServerContext&			m_context;
			const ServerBlock		*m_serverBlock;
			const ServerLocation	*m_location;
		};
}

#endif