
// Cgi only uses a single new line character

# include "../../../GenericUtils/Buffer/Buffer.hpp"
# include "../../../GenericUtils/BufferView/BufferView.hpp"

# include <cstring>
# include <vector>
# include <iostream>
# include <algorithm>

const char* good= "Content-Type: text/html\n"
					"\n"
					"Hello World"
					;

# define CGI_LINE_SEPARATOR '\n'
# define CGI_HEADER_SEPARATOR ": "
# define CGI_VALUE_SEPARATOR "; "

# define CGI_SUCCESS 200

# define CGI_MAX_HEADER_SIZE 1024

struct CgiHeader
{
	BufferView key;
	BufferView value;

	bool operator<(const CgiHeader& other) const
	{
		return (key < other.key);
	}

	bool operator==(const CgiHeader& other) const
	{
		return (key == other.key);
	}

	bool operator!=(const CgiHeader& other) const
	{
		return (key != other.key);
	}
};

struct CompareHeaders
{
	bool operator()(const CgiHeader& a, const CgiHeader& b) const
	{
		return (a.key < b.key);
	}
};

struct FindHeaders
{
	bool operator()(const CgiHeader& a, const CgiHeader& b) const
	{
		return (a.key < b.key);
	}
};

//static const char* mandatoryHeaders[] =
//{
//	"Content-Type",
//	"Location",
//	"Status"
//};

template<typename T>
int binSearch(const std::vector<T>& target, const T& value)
{
	int low = 0;
	int high = target.size() - 1;
	int mid;

	if (target.size() == 0)
		return (-1);

	while (low < high)
	{
		mid = low + ((high - low) / 2);
		if (target[mid] == value)
			return (mid);
		else if (value < target[mid])
			high = mid - 1;
		else
			low = mid + 1;
	}

	if (value != target[low])
		return (-1);

	return (low);
}


// headers that interfere with http communication, to be verified by the http module
static const char* forbiddenHeaders[] =
{
	"Content-Length",
	"Transfer-Encoding",
	"Connection",
	"Upgrade"
};

namespace Cgi
{
	class Response
	{
		public:

			Response(Buffer& client) : m_state(Response::HEADERS), m_totalHeaderBytes(0), m_client(client), m_hasBody(false) {}
			~Response() {}

			enum Status
			{
				PASS,
				FAIL,
				INSUFFICIENT,
			};

			Status parse(BufferView& view);
			
			const std::vector<CgiHeader>& getHeaders() const { return (m_headers); }
			bool hasBody() const { return (m_hasBody); }

			

		private:
			enum State
			{
				HEADERS,
				TEMP_BODY,
				BODY,
				FINISH
			};
			
			State						m_state;
			int							m_totalHeaderBytes;
			std::vector<CgiHeader> 		m_headers;
			Buffer&						m_client;
			bool						m_hasBody;
			BufferView					m_tempBody;

			Status	mf_validateHeaders();
			Status	mf_parseHeaders(BufferView& view);
			Status	mf_parseBody(BufferView& view);
	};
};

Cgi::Response::Status Cgi::Response::mf_parseBody(BufferView& view)
{
	(void)view;
	return (Response::PASS);
}

Cgi::Response::Status Cgi::Response::mf_validateHeaders()
{
	std::sort(m_headers.begin(), m_headers.end(), CompareHeaders());
	
	int status = binSearch(m_headers, (CgiHeader){BufferView("Status"), BufferView()});
	int contentType = binSearch(m_headers, (CgiHeader){BufferView("Content-Type"), BufferView()});
	int location = binSearch(m_headers, (CgiHeader){BufferView("Location"), BufferView()});

	if (status == -1 && contentType == -1 && location == -1)
	{
		std::cout << "lacking at least one mandatory header" << std::endl;
		return (Response::FAIL);
	}
	
	if (status == -1)
		status = CGI_SUCCESS;
	else
		status = std::atoi(m_headers[status].value.data());

	if (location != -1)
		m_hasBody = true;

	for (size_t i = 0; i < sizeof(forbiddenHeaders) / sizeof(forbiddenHeaders[0]); ++i)
	{
		if (binSearch(m_headers, (CgiHeader){BufferView(forbiddenHeaders[0]), BufferView()}) != -1)
		{
			std::cout << "forbidden header included: " << forbiddenHeaders[0] << std::endl;
			return (Cgi::Response::FAIL);
		}
	}
	std::cout << "headers validated" << std::endl;
	return (Response::PASS);
}

Cgi::Response::Status Cgi::Response::mf_parseHeaders(BufferView& view)
{
	size_t pos = view.find(CGI_LINE_SEPARATOR);
	if (pos == BufferView::npos)
	{
		return (Response::INSUFFICIENT);
	}


	BufferView line = view.substr(0, pos);
	size_t next = line.find(CGI_HEADER_SEPARATOR, 2, 0);
	if (next == BufferView::npos)
	{
		std::cout << "no coma separator found in header" << std::endl;
		return (Response::FAIL);
	}
	BufferView key = line.substr(0, next);
	BufferView value = line.substr(next + 2, line.size() - next - 2);
	if (binSearch(m_headers, (CgiHeader){key, value}) != -1)
	{
		std::cout << "duplicate header found: " << key << std::endl;
		return (Response::FAIL);
	}

	m_headers.push_back((CgiHeader){key, value});	// save header
	std::cout << "key: '" <<  m_headers[0].key << "', value: '" << m_headers[0].value << "'" << std::endl;

	size_t end = view.find(CGI_LINE_SEPARATOR, pos + 1);
	std::cout << "first newline found at: " << pos << std::endl;
	std::cout << "second newline found at: " << end << std::endl;
	if (end == pos + 1)
	{
		std::cout << "found second newline, " << std::endl;
		view = view.substr(pos + 2, view.size() - pos - 2);
		if (mf_validateHeaders() == Response::FAIL)
			return (Response::FAIL);
		std::cout << "m_hasbody" << m_hasBody << std::endl;
		if (view.size() > 0)
		{
			if (m_hasBody == true)
				m_tempBody = view;
			else
				return (Response::FAIL);
		}
		std::cout << "should reach here" << std::endl;
		m_state = Response::BODY;
		return (Response::PASS);
	}

	view = view.substr(line.size() + 1, view.size() - line.size() - 1); // move view to one past end of line
	
	return (Response::PASS);
}

Cgi::Response::Status	Cgi::Response::parse(BufferView& view)
{

	if (view.size() == 0)
	{
		m_state = Response::FINISH;
		//eof received, inform the client there is no more info comming up
	};

	while (view.size() > 0)
	{
		switch ((int)m_state)
		{
			case Response::HEADERS:
			{
				int retStatus = mf_parseHeaders(view);
				if (retStatus == Response::FAIL)
					return (Response::FAIL);
				else if (retStatus == Response::INSUFFICIENT)
				{
					std::cout << "insufficient data for headers" << std::endl;
					return (Response::INSUFFICIENT);
				}
				break ;
			}
			case Response::BODY:
			{
				if (mf_parseBody(view) == Response::FAIL)
					return (Response::FAIL);
				break ;
			}
			case Response::FINISH:
				break ;
		}
	}
	return (Response::PASS);
}

int main(void)
{
	Buffer client;
	Buffer readBuffer;
	BufferView readView;
	Cgi::Response response(client);

	readBuffer.push("Stat", std::strlen("Stat"));
	readView = readBuffer.view();
	response.parse(readView);

	readBuffer.push("us: 200\n\n", std::strlen("us: 200\n\n"));
	readView = (BufferView){readView.data(), readView.size() + std::strlen("us: 200\n\n")};
	response.parse(readView);
	std::cout << readView.size() << "  '" << readView << "'" << std::endl;
	response.parse(readView);

	return (0);
}