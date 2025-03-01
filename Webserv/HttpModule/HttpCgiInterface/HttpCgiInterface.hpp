

#ifndef HTTPCGIINTERFACE_HPP

# define HTTPCGIINTERFACE_HPP

# include "../HttpDefinitions.hpp"
# include "../../CgiModule/CgiNamespace.h"

# include "../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"

namespace Http { class CgiGateway;}
namespace Cgi { class Module;}

namespace Http
{
    class CgiInterface
    {
        public:
            CgiInterface(Cgi::Module& cgiModule, const size_t numberOfResponses);
            ~CgiInterface();

            Http::CgiGateway*   acquireGateway();
            void                releaseGateway(Http::CgiGateway& gateway);

        private:
            Cgi::Module&                            m_cgiModule;
            HeapArray<Http::CgiGateway>             m_cgiGateways;
            HeapCircularQueue<Http::CgiGateway*>    m_availableGateways;

            CgiInterface(const CgiInterface& other);
            CgiInterface& operator=(const CgiInterface& other);
    };


    
}



#endif