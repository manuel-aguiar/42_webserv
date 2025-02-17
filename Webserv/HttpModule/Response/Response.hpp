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
class Buffer;
namespace Http {class Request; class Connection;}

// move to relevant place
#define DEFAULT_ERROR_PAGES_PATH "Webserv/StaticAssets/ErrorPages/error_"

namespace Http
{
	class Response
	{
		public:
			Response(Http::Request& myRequest, ServerContext &context);
			
			std::string	&getMessage(size_t statusCode);

			void	fillWriteBuffer(Buffer& writeBuffer); // give me all data you can, until Buffer::capacity()

		private:
			void	generateResponse(int statusCode);

			Http::Request			&m_myRequest;
			File					*m_file;

			ServerContext&			m_context;
			const ServerBlock		*m_serverBlock;
			const ServerLocation	*m_location;
	};
}

#endif
