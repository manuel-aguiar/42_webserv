

#ifndef HTTPCGIINTERFACE_HPP

# define HTTPCGIINTERFACE_HPP

# include "../HttpDefinitions.hpp"
# include "../../CgiModule/CgiNamespace.h"

# include "../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"

namespace Http { class CgiResponse;}
namespace Http { class CgiResponse;}
namespace Cgi { class Module;}

namespace Http
{
	class CgiInterface
	{
		public:
			CgiInterface(Cgi::Module& cgiModule, const size_t numberOfResponses);
			~CgiInterface();

			Http::CgiResponse*   acquireGateway();
			void                releaseGateway(Http::CgiResponse& gateway);

		private:
			Cgi::Module&                                m_cgiModule;
			HeapArray<Http::CgiResponse>                m_cgiGateways;
			HeapCircularQueue<Http::CgiResponse*>       m_availableGateways;

			CgiInterface(const CgiInterface& other);
			CgiInterface& operator=(const CgiInterface& other);
	};


	
}



#endif
