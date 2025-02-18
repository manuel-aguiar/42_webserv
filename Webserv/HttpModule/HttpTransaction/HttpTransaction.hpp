

#ifndef HTTPTRANSACTION_HPP

# define HTTPTRANSACTION_HPP

# include "../HttpRequest/HttpRequest.hpp"
# include "../HttpResponse/HttpResponse.hpp"

namespace Http
{
	struct Transaction
	{
		Transaction(Http::Connection& connection);
		Transaction(const Transaction& other);
		~Transaction();
		Transaction& operator=(const Transaction& other);


		Http::Request	request;
		Http::Response	response;
	};
}

#endif