
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

# define CGI_MAX_HEADER_SIZE 8192

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

namespace Parsing
{
	enum State
	{
		START,
		HEADERS,
		BODY,
		FINISH
	};
}

namespace Cgi
{
	class Response
	{
		public:

			Response(Buffer& client) : m_state(State::START), m_totalHeaderBytes(0), m_client(client) {}
			~Response() {}

			enum Status
			{
				PASS,
				FAIL
			};

			Status parse(BufferView& view);

		private:
			enum State
			{
				START,
				HEADERS,
				BODY,
				FINISH
			};
			


			State						m_state;
			int							m_totalHeaderBytes;
			std::vector<CgiHeader> 		m_headers;
			Buffer&						m_client;
			std::vector<unsigned char>	m_tempBody;


			Status	mf_parseHeaders(BufferView& view);
			Status	mf_parseBody(BufferView& view);
	};
};

Cgi::Response::Status Cgi::Response::mf_parseHeaders(BufferView& view)
{
	size_t pos = view.find(CGI_LINE_SEPARATOR);
	if (pos == BufferView::npos)
	{
		std::cout << "no newline found" << std::endl;
		//return (1);
	}
	size_t end = view.find(CGI_LINE_SEPARATOR, pos);

	if (end == 0)
	{
		view = view.substr(pos + 1, view.size() - pos - 1);
		m_state = State::BODY;
	}

	BufferView line = view.substr(0, pos);
	size_t next = line.find(CGI_HEADER_SEPARATOR, 2, 0);
	if (next == BufferView::npos)
	{
		return (Status::FAIL);
		std::cout << "no coma separator found " << std::endl;
		//return (2);
	}
	BufferView key = line.substr(0, next);
	BufferView value = line.substr(next + 2, line.size() - next - 2);

	if (binSearch(m_headers, (CgiHeader){key, value}) != -1)
	{
		return (Status::FAIL);
		std::cout << "duplicate header found: " << key << std::endl;
		//return (3);
	}

	m_headers.push_back((CgiHeader){key, value});	// save header

	view = view.substr(line.size() + 1, view.size() - line.size() - 1); // move view to one past end of line
}

Cgi::Response::Status	Cgi::Response::parse(BufferView& view)
{

	if (view.size() == 0)
	{
		m_state = State::FINISH;
		//eof received, inform the client there is no more info comming up
	};

	while (view.size() > 0)
	{
		switch (m_state)
		{
			case State::HEADERS:
			{
				if (mf_parseHeaders(view) == Status::FAIL)
					return (Status::FAIL);
				break ;
			}
			case State::BODY:
			{
				if (mf_parseBody(view) == Status::FAIL)
					return (Status::FAIL);
				break ;
			}
			case State::FINISH:
				break ;
		}
	}
	
}


int main(void)
{
	std::vector<CgiHeader> headers;
	BufferView view(good);

	Parsing::State state = Parsing::HEADERS;

	while (1)
	{
		size_t pos = view.find(CGI_LINE_SEPARATOR);
		if (pos == BufferView::npos)
		{
			std::cout << "no newline found" << std::endl;
			return (1);
		}
		size_t end = view.find(CGI_LINE_SEPARATOR, pos);

		if (end == 0)
		{
			view = view.substr(pos + 1, view.size() - pos - 1);
			state = Parsing::BODY;
			break ; // end of headers
		}

		BufferView line = view.substr(0, pos);
		size_t next = line.find(CGI_HEADER_SEPARATOR, 2, 0);
		if (next == BufferView::npos)
		{
			std::cout << "no coma separator found " << std::endl;
			return (2);
		}
		BufferView key = line.substr(0, next);
		BufferView value = line.substr(next + 2, line.size() - next - 2);

		if (binSearch(headers, (CgiHeader){key, value}) != -1)
		{
			std::cout << "duplicate header found: " << key << std::endl;
			return (3);
		}

		headers.push_back((CgiHeader){key, value});	// save header

		view = view.substr(line.size() + 1, view.size() - line.size() - 1); // move view to one past end of line
	}
	
	std::sort(headers.begin(), headers.end(), CompareHeaders());
	
	int status = binSearch(headers, (CgiHeader){BufferView("Status"), BufferView()});
	int contentType = binSearch(headers, (CgiHeader){BufferView("Content-Type"), BufferView()});
	int location = binSearch(headers, (CgiHeader){BufferView("Location"), BufferView()});

	if (status == -1 && contentType == -1 && location == -1)
	{
		std::cout << "lacking at least one mandatory header" << std::endl;
		return (3);
	}
	
	if (status == -1)
		status = CGI_SUCCESS;
	else
		status = std::atoi(headers[status].value.data());

	for (size_t i = 0; i < sizeof(forbiddenHeaders) / sizeof(forbiddenHeaders[0]); ++i)
	{
		if (binSearch(headers, (CgiHeader){BufferView(forbiddenHeaders[0]), BufferView()}) != -1)
		{
			std::cout << "forbidden header included: " << forbiddenHeaders[0] << std::endl;
			return (3);
		}
	}
	
	if (contentType == -1 && view.size() != 0)
	{
		std::cout << "no content type header but content found" << std::endl;
		return (4);
	}
	/////////////////////////////////

	// SUCCESS

	std::cout << "status: " << status << std::endl;
	std::cout << "headers: " << std::endl;
	for (size_t i = 0; i < headers.size(); ++i)
	{
		std::cout << "\t" << headers[i].key << ": " << headers[i].value << std::endl;
	}

	if (contentType != -1)
	{
		std::cout << "body: \t" << view << std::endl;
	}

	return (0);
}

int main(void)
{
	Buffer client;
	Buffer readBuffer;
	BufferView readView;
	Cgi::Response response(client);

	readBuffer.push("Stat", 4);
	readView = readBuffer.view();

	response.parse(readView);

	readBuffer.push("us: 200\n", 8);
	readView = (BufferView){readView.data(), readView.size() + 8};

	response.parse(readView);

	return (0);
}