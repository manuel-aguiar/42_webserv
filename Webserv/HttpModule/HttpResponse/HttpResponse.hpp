#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "./HttpResponse_DirectoryListing.hpp"
# include "../HttpDefinitions.hpp"
# include "../../GenericUtils/Files/File.hpp"

# include <iostream>
# include <sstream>
# include <iomanip>
# include <ctime>
# include <string>

// TESTING
# include "../HttpRequest/HttpRequest.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../ServerConfig/ServerConfig/ServerConfig.hpp"
# include "../../ServerConfig/ServerLocation/ServerLocation.hpp"
# include "../../ServerConfig/ServerBlock/ServerBlock.hpp"


// forward declarations
class ServerBlock;
class ServerLocation;
class ServerContext;
class BaseBuffer;
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

			typedef enum
			{
				WAITING, // have nothing to push, no sign transaction is finished
				WRITING, // pushed data to buffer
				FINISHED // transaction is finished
			}	Status;

			const char*			getMessage(int statusCode);
			Status  			getStatus() const;

			Response::Status	fillWriteBuffer(BaseBuffer& writeBuffer); // give me all data you can, until Buffer::capacity()

			void    			reset(); // reset the response to its initial state

		private:
			void				generateResponse(int statusCode);
			std::string			generateStatusLine(int statusCode);
			std::string			generateHeaderString();
			std::string 		generateDefaultErrorPage(int StatusCode);

			// Debatable
			void				setGetRqContentType(std::map<std::string, std::string> &m_headers, int fileExtension);


			Http::Request&		m_myRequest;
			Http::Connection&	m_connection;
			File*				m_file;
			ServerBlock*		m_serverBlock;
			ServerLocation*		m_location;
			Status				m_status;
			std::map<std::string, std::string>	m_headers;

			std::string			m_pendingWrite;		// cache data that you generated but couldn't write
	};
};

#endif
