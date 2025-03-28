# include "HttpResponse.hpp"
# include "../../ServerConfig/ServerLocation/ServerLocation.hpp"
# include "../../ServerConfig/ServerBlock/ServerBlock.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include <arpa/inet.h>

static bool isAbsoluteUrl(const std::string& path)
{
    return (path.find("http://") == 0 || path.find("https://") == 0);
}

static bool isRelativePath(const std::string& path)
{
    return (!path.empty() && path[0] != '/');
}

static std::string buildLocationHeader(const std::string& host, const std::string& path)
{
    if (host.empty())
        return (isRelativePath(path) ? "/" + path : path);

    std::string location = "http://" + host;
    if (isRelativePath(path))
        location += "/";

    location += path;
    return (location);
}

static bool shouldPreserveMethod(Http::Status::Number status)
{
    return (status == Http::Status::TEMPORARY_REDIRECT ||
            status == Http::Status::PERMANENT_REDIRECT);
}

static bool shouldConvertToGet(Http::Status::Number status)
{
    return (status == Http::Status::MOVED_PERMANENTLY ||
            status == Http::Status::FOUND ||
            status == Http::Status::SEE_OTHER);
}
namespace Http
{
    // documentation:
    // https://www.rfc-editor.org/rfc/rfc9110.html#name-redirection-3xx
    bool Response::mf_checkRedirect()
    {
		if (m_responseData.serverLocation == NULL)
			return (false);

        const std::pair<int, std::string>& locReturn = m_responseData.serverLocation->getReturn();

        // only handling specific redirects
        switch (locReturn.first)
        {
            case Http::Status::MOVED_PERMANENTLY:  // 301
            case Http::Status::FOUND:              // 302
            case Http::Status::SEE_OTHER:          // 303
            case Http::Status::TEMPORARY_REDIRECT: // 307
            case Http::Status::PERMANENT_REDIRECT: // 308
                break;
            default:
                return (false);
        }

        // Validate redirect path
        const std::string& redirectPath = locReturn.second;
        ASSERT_EQUAL(redirectPath.empty(), false, "Redirect path is empty! Config error");

        // Set response status and type
        m_responseData.requestStatus = static_cast<Http::Status::Number>(locReturn.first);
        m_responseData.responseType = ResponseData::REDIRECT;

        // building the location first
        if (isAbsoluteUrl(redirectPath))
            m_responseData.headers["location"] = redirectPath;
        else
        {
            std::string host;
            if (m_responseData.requestData &&
                     m_responseData.requestData->headers.find("Host") != m_responseData.requestData->headers.end())
                host = m_responseData.requestData->headers.at("Host"); // host takes precedence
            else if (!m_responseData.serverBlock->getServerNames().empty())
                host = *m_responseData.serverBlock->getServerNames().begin(); // over server names

            m_responseData.headers["location"] = buildLocationHeader(host, redirectPath);
        }

        // preserve method or convert based on the status
        if (m_responseData.requestData)
        {
            if (shouldPreserveMethod(m_responseData.requestStatus)) // 307, 308
            {
                m_responseData.headers["allow-method"] = m_responseData.requestData->method;
                if (m_responseData.serverLocation->getPath() == "/")
                    m_responseData.headers["location"] = m_responseData.headers["location"] + m_responseData.requestData->uri;
            }
            else if (shouldConvertToGet(m_responseData.requestStatus) &&
                     m_responseData.requestData->method == "POST")
            {
                m_responseData.headers["x-http-method-override"] = "GET";
            }
        }

        // add cache-control header based on the status (this is for primitive stuff)
        if (m_responseData.requestStatus == Http::Status::MOVED_PERMANENTLY ||
            m_responseData.requestStatus == Http::Status::PERMANENT_REDIRECT)
        {
            m_responseData.headers["cache-control"] = "public, max-age=31536000";
        }
        else
        {
            m_responseData.headers["cache-control"] = "no-cache, no-store, must-revalidate";
        }

        return (true);
    }
}
