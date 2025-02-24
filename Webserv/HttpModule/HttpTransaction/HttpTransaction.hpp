

#ifndef HTTPTRANSACTION_HPP

# define HTTPTRANSACTION_HPP

# include "../HttpRequest/HttpRequest.hpp"
# include "../HttpResponse/HttpResponse.hpp"

class ServerContext;

namespace Http
{
	struct Transaction
	{
		Transaction(ServerContext& context);
		Transaction(const Transaction& other);
		~Transaction();
		Transaction& operator=(const Transaction& other);

		void			reset();

		Http::Request	request;
		Http::Response	response;
	};
}

#endif
