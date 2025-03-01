# include "HttpResponse.hpp"
# include "../../ServerConfig/ServerLocation/ServerLocation.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"

namespace Http
{
    bool Response::mf_checkRedirect()
    {
        ASSERT_EQUAL(m_responseData.serverLocation != NULL, true, "Server location is NULL");

        const std::pair<int, std::string>& locReturn = m_responseData.serverLocation->getReturn();
        if (locReturn.first <  Http::Status::MOVED_PERMANENTLY || \
            locReturn.first > Http::Status::PERMANENT_REDIRECT) return (false);

        m_responseData.requestStatus = static_cast<Http::Status::Number>(locReturn.first);
        std::string redirectPath = locReturn.second;
        ASSERT_EQUAL(redirectPath.empty(), false, "Redirect path is empty! Config error");

        if (redirectPath.find("http://") == 0) {
            m_responseData.headers["location"] = redirectPath;
        }
        else if (!redirectPath.empty() && redirectPath[0] == '/') {
            m_responseData.headers["location"] = 'http://' + m_responseData.serverLocation->getServerName() + redirectPath;
        }
        else {
            m_responseData.headers["location"] = 'http://' + m_responseData.serverLocation->getServerName() + '/' + redirectPath;
        }

        m_responseData.responseType = ResponseData::REDIRECT;
        return (true);
    }
}