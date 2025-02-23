

#ifndef CGIRESPONSE_HPP

# define CGIRESPONSE_HPP

// Project headers
# include "../../GenericUtils/BufferView/BufferView.hpp"
# include "../CgiNamespace.h"

// C++ headers
# include <vector>

# define CGI_LINE_SEPARATOR '\n'
# define CGI_HEADER_SEPARATOR ": "
# define CGI_VALUE_SEPARATOR "; "

# define CGI_SUCCESS 200
# define CGI_FAILURE 500

# define CGI_MAX_HEADER_SIZE 1024


// forward declarations
class BaseBuffer;

namespace Cgi
{

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
	class HeaderData
	{
		public:

			HeaderData();
			~HeaderData();

			enum Status
			{
				PASS,
				FAIL,
				KEEP_PARSING,
				NEED_MORE_DATA
			};

			enum ParsingState
			{
				START,
				HEADERS,
				FINISH
			};
			
			void                            reset();

			Status                          parse(BaseBuffer& buffer);
			
			ParsingState					getParsingState() const;	
			const std::vector<Cgi::Header>& getHeaders() const;
			const BufferView&               getTempBody() const;
			int							  	getStatusCode() const;
			bool                            hasBody() const;		

		private:
			
			ParsingState						m_state;
			int                         m_statusCode;
			int							m_totalParsedBytes;
			std::vector<Cgi::Header> 	m_headers;
			bool						m_hasBody;
			BufferView					m_tempBody;

			Status	mf_validateHeaders();
			Status	mf_parseHeaders(BufferView& view);
			Status	mf_parseBody(BufferView& view);
			Status	mf_setStatus(const Status status, const int statusCode);
	};
};

#endif