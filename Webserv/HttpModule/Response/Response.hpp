

#ifndef HTTPRESPONSE_HPP

# define HTTPRESPONSE_HPP

// forward declarations
namespace Http { class Request; class Connection; }
class Buffer;

enum SSStatus
{
	COMPLETED,
};

namespace Http
{
	class Response
	{
		public:
			Response(Http::Request& request, Http::Connection& connection);
			~Response();
			Response(const Response& other);
			Response& operator=(const Response& other);

			SSStatus 	status() const;
			void 		fillWriteBuffer(Buffer& writeBuffer);
	};
};

#endif