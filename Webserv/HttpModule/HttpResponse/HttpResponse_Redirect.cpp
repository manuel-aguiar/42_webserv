# include "HttpResponse.hpp"
# include "../../ServerConfig/ServerLocation/ServerLocation.hpp"
# include "../../ServerConfig/ServerBlock/ServerBlock.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include <arpa/inet.h>

namespace Http
{
    bool Response::mf_checkRedirect()
    {
        ASSERT_EQUAL(m_responseData.serverLocation != NULL, true, "Server location is NULL");
        ASSERT_EQUAL(m_responseData.serverBlock != NULL, true, "Server block is NULL");

        const std::pair<int, std::string>& locReturn = m_responseData.serverLocation->getReturn();
        std::cout << "Checking redirect" << "status: " << locReturn.first;
        if (locReturn.first <  Http::Status::MOVED_PERMANENTLY || \
            locReturn.first > Http::Status::PERMANENT_REDIRECT) return (false);

        m_responseData.requestStatus = static_cast<Http::Status::Number>(locReturn.first);
        std::string redirectPath = locReturn.second;
        ASSERT_EQUAL(redirectPath.empty(), false, "Redirect path is empty! Config error");

        m_responseData.responseType = ResponseData::REDIRECT;
        std::cout << "Redirecting to: " << redirectPath << std::endl;

        // Already an absolute URL - use as is
        if (redirectPath.find("http://") == 0) { // only http supported
            m_responseData.headers["location"] = redirectPath;
            return (true);
        }

        // using the first server name as the host
        std::string host;
        if (!m_responseData.serverBlock->getServerNames().empty()) {
            host = *m_responseData.serverBlock->getServerNames().begin();
        }

        if (host.empty() && m_responseData.requestData &&
            m_responseData.requestData->headers.find("Host") != m_responseData.requestData->headers.end()) {
            host = m_responseData.requestData->headers.at("Host");
        }

        std::string location;
        if (!host.empty()) {
            location = "http://" + host;

            if (redirectPath[0] != '/') { // was a relative path, make it absolute
                location += "/";
            }
        }

        else if (redirectPath[0] != '/') { // was a relative path, make it absolute
            location = "/";
        }

        // final merge
        location += redirectPath;

        m_responseData.headers["location"] = location;
        return (true);
    }
}