#include "HttpResponse.hpp"
#include "../../GenericUtils/StringUtils/StringUtils.hpp"


namespace Http
{
	void	Response::mf_addContentHeaders(const size_t size, const std::string mimeType)
	{
		m_responseData.headers.insert(std::make_pair("content-type", mimeType));
		m_responseData.headers.insert(std::make_pair("content-length", StringUtils::to_string(size)));
	}

	bool	Response::mf_addCacheControlHeaders()
	{
		// Last-Modified
		char	buffer[32];
		time_t	lastmodified = m_file.getLastModified();
		
		strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&lastmodified));
		m_responseData.headers.insert(std::make_pair("last-modified", buffer));
		
		// ETag
		std::string	etag = mf_generateEtag(m_file, lastmodified);
		std::string	etag = mf_generateEtag(m_file, lastmodified);
		m_responseData.headers.insert(std::make_pair("etag", etag));
		std::map<std::string, std::string>::const_iterator it = m_responseData.requestData->headers.find("If-None-Match");
		if (it != m_responseData.requestData->headers.end() && it->second == etag)
		{
			m_responseData.requestStatus = Http::Status::NOT_MODIFIED;
			m_fillBody = NULL;
			return (true);
		}
		return (false);
	}

	bool	Response::mf_addHeader(const std::string& key, const std::string& value)
	{
		if (m_responseData.headers.find(key) != m_responseData.headers.end())
			return (false);
		m_responseData.headers.insert(std::make_pair(key, value));
		return (true);
	}
}
