#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../HttpDefinitions.hpp"
# include "../../GenericUtils/Files/File.hpp"

// DELETE THIS
# include "../HttpRequest/HttpRequest.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../ServerConfig/ServerConfig/ServerConfig.hpp"
# include "../../ServerConfig/ServerLocation/ServerLocation.hpp"
# include "../../ServerConfig/ServerBlock/ServerBlock.hpp"

class ServerContext;
class HttpRequest;

// move to relevant place
#define DEFAULT_ERROR_PAGES_PATH "Webserv/StaticAssets/ErrorPages/error_"

namespace Http {
	class Response {
		public:
			// Response(Http::Connection& myConnection, Http::Request& myRequest, ServerContext& context);	
			Response(HttpRequest& myRequest, ServerContext &context);
			
			std::string	&getMessage(size_t statusCode);
		private:
			void	generateResponse(int statusCode);

			//Http::Connection&		m_myConnection;
			//Http::Request&			m_myRequest;
			HttpRequest				&m_myRequest;
			File					*m_file;

			ServerContext&			m_context;
			const ServerBlock		*m_serverBlock;
			const ServerLocation	*m_location;
		};
}

#endif
