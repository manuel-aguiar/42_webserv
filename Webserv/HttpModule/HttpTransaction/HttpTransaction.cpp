

# include "HttpTransaction.hpp"

namespace Http
{
	Transaction::Transaction(Http::Connection& connection)
		: request(connection),
		  response(connection, request) {}

	Transaction::Transaction(const Transaction& other)
		: request(other.request),
		  response(other.response) {}

	Transaction& Transaction::operator=(const Transaction& other)
	{
		if (this == &other)
			return (*this);

		request = other.request;
		response = other.response;
		
		return (*this);
	}

	Transaction::~Transaction() {}
}
