

#ifndef HTTPREQUEST_HPP

# define HTTPREQUEST_HPP

class Buffer;

namespace Http { class Connection;}

enum SSStatus
{
	COMPLETED,
};

namespace Http
{
	class Request
	{
		public:
			Request(Http::Connection& connection);
			~Request();
			Request(const Request& other);
			Request& operator=(const Request& other);

			SSStatus 	status() const;
			void 		parse(const Buffer& buffer);

			Http::Connection&
						getConnection();
			
			const std::string& getMsgBody() const;

		private:
			Http::Connection& m_connection;
	};
}


#endif