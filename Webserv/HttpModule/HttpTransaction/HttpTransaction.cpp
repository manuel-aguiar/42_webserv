

# include "HttpTransaction.hpp"

namespace Http
{
	Transaction::Transaction(ServerContext& context)
		: request(context),
		  response(context) {}

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
