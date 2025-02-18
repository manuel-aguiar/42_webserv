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

// forward declarations
class ServerBlock;
class ServerLocation;
class ServerContext;
class Buffer;
namespace Http {class Request;}

namespace Http
{
	class Response
	{
		public:
			Response(Http::Connection& conn, Http::Request& myRequest);
			Response(const Response& other);
			~Response();
			Response& operator=(const Response& other);
			
			const char* getMessage(int statusCode);

			void	fillWriteBuffer(Buffer& writeBuffer); // give me all data you can, until Buffer::capacity()

			void    reset(); // reset the response to its initial state

		private:
			void	generateResponse(int statusCode);

			Http::Request&			m_myRequest;
			Http::Connection&		m_connection;
			File*					m_file;
			ServerBlock*			m_serverBlock;
			ServerLocation*			m_location;

			std::string				m_pendingWrite;		// cache data that you generated but couldn't write
	};
};

#endif
