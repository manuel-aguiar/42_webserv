

#ifndef HTTPCONNECTION_HPP

# define HTTPCONNECTION_HPP

//C++ headers

# include <vector>
# include <queue>

class Connection;
class HttpRequest;
class HttpResponse;
class Event;


class HttpConnection
{
	public:
		HttpConnection(Connection* newConnection);
		~HttpConnection();



	private:

		size_t							m_timeout;
		Connection*						m_connection;
		std::queue<HttpRequest>			m_requests;
		std::queue<HttpResponse>		m_responses;


		HttpConnection(const HttpConnection& copy);
		HttpConnection& operator=(const HttpConnection& assign);
};



#endif
