

# include "HttpCgiInterface.hpp"
# include "../HttpCgiGateway/HttpCgiGateway.hpp"

namespace Http
{
    CgiInterface::CgiInterface(Cgi::Module& module, const size_t numberOfRequests)
        : m_cgiModule(module)
        , m_cgiGateways(numberOfRequests)
        , m_availableGateways(numberOfRequests)
    {
        for (size_t i = 0; i < numberOfRequests; ++i)
        {
            m_cgiGateways.emplace_back(module);
            m_availableGateways.push_back(&m_cgiGateways[i]);
        }
    }

    CgiInterface::~CgiInterface()
    {
        //nithing, auto calls destructors
    }

    CgiInterface::CgiInterface(const CgiInterface& copy)
        : m_cgiModule(copy.m_cgiModule)
        , m_cgiGateways(copy.m_cgiGateways)
        , m_availableGateways(copy.m_availableGateways)
    {
        // nothing
    }

    CgiInterface& 
    CgiInterface::operator=(const CgiInterface& assign)
    {
        if (this == &assign)
            return (*this);

        ASSERT_EQUAL(&m_cgiModule, &assign.m_cgiModule, "CgiInterface::operator=() - module mismatch");

        m_cgiGateways = assign.m_cgiGateways;
        m_availableGateways = assign.m_availableGateways;

        return (*this);
    }

    Http::CgiGateway*
    CgiInterface::acquireGateway()
    {
        if (m_availableGateways.size() == 0)
            return (NULL);
        Http::CgiGateway* gateway = m_availableGateways.front();
        m_availableGateways.pop_front();
        return (gateway);
    }

    void
    CgiInterface::releaseGateway(Http::CgiGateway& gateway)
    {
        // check if belongs to this interface
        ASSERT_EQUAL(&gateway >= &m_cgiGateways[0] && &gateway <= &m_cgiGateways[m_cgiGateways.size() - 1], true, "CgiInterface::releaseGateway() - gateway out of range");
        
        gateway.reset();
        m_availableGateways.push_back(&gateway);
    }
}