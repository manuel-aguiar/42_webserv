

#ifndef CGIRESPONSE_HPP

# define CGIRESPONSE_HPP

// Project headers
# include "../../../GenericUtils/BufferView/BufferView.hpp"

// C++ headers
# include <vector>

# define CGI_LINE_SEPARATOR '\n'
# define CGI_HEADER_SEPARATOR ": "
# define CGI_VALUE_SEPARATOR "; "

# define CGI_SUCCESS 200

# define CGI_MAX_HEADER_SIZE 1024


// forward declarations
class BaseBuffer;

namespace Cgi
{
    struct Header
    {
        BufferView key;
        BufferView value;
    
        bool operator<(const Header& other) const
        {
            return (key < other.key);
        }
    
        bool operator==(const Header& other) const
        {
            return (key == other.key);
        }
    
        bool operator!=(const Header& other) const
        {
            return (key != other.key);
        }
    };
};


struct CompareHeaders
{
	bool operator()(const Cgi::Header& a, const Cgi::Header& b) const
	{
		return (a.key < b.key);
	}
};

namespace Cgi
{
	class Response
	{
		public:

			Response(BaseBuffer& client);
			~Response();

			enum Status
			{
				PASS,
				FAIL,
				INSUFFICIENT,
			};

			enum State
			{
				HEADERS,
				TEMP_BODY,
				BODY,
				FINISH
			};

			Status parse(BufferView& view);
			
			const std::vector<Cgi::Header>& getHeaders() const { return (m_headers); }
			bool hasBody() const { return (m_hasBody); }

			

		private:
			
			State						m_state;
			int							m_totalHeaderBytes;
			std::vector<Cgi::Header> 		m_headers;
			BaseBuffer&					m_client;
			bool						m_hasBody;
			BufferView					m_tempBody;

			Status	mf_validateHeaders();
			Status	mf_parseHeaders(BufferView& view);
			Status	mf_parseBody(BufferView& view);
	};
};


#endif