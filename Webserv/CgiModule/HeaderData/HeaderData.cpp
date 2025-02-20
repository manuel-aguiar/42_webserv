

# include "HeaderData.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"

# include <cstring>
# include <vector>
# include <algorithm>


# include <iostream>            //DEBUG, erase afterwards

static const char* forbiddenHeaders[] =
{
	"Connection",
	"Content-Length",
	"Transfer-Encoding",
	"Upgrade"
};

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


HeaderData::HeaderData() 
: m_state               (HeaderData::START)
, m_statusCode          (-1)
, m_totalParsedBytes    (0)
, m_hasBody             (false) {}


HeaderData::~HeaderData() {}



void
HeaderData::reset()
{
	m_state = HeaderData::START;
	m_statusCode = -1;
	m_totalParsedBytes = 0;
	m_hasBody = false;
	m_headers.clear();
	m_tempBody = BufferView();
}


HeaderData::Status
HeaderData::mf_parseBody(BufferView& view)
{
	(void)view;
	return (HeaderData::PASS);
}

HeaderData::Status
HeaderData::mf_validateHeaders()
{
	std::sort(m_headers.begin(), m_headers.end(), CompareHeaders());
	
	for (size_t i = 1; i < m_headers.size(); ++i)
	{
		if (m_headers[i - 1].key == m_headers[i].key)
		{
			//std::cout << "duplicate header found: " << m_headers[i].key << std::endl;
			return (mf_setStatus(HeaderData::FAIL, CGI_FAILURE));
		}
	}

	int contentType = binSearch(m_headers, (Cgi::Header){BufferView("Content-Type"), BufferView()});
	int location    = binSearch(m_headers, (Cgi::Header){BufferView("Location"), BufferView()});

	if (m_statusCode == -1 && contentType == -1 && location == -1)
	{
		//std::cout << "lacking at least one mandatory header" << std::endl;
		return (mf_setStatus(HeaderData::FAIL, CGI_FAILURE));
	}
	
	if (m_statusCode == -1)
		m_statusCode = CGI_SUCCESS;

	if (contentType != -1)
		m_hasBody = true;

	for (size_t i = 0; i < sizeof(forbiddenHeaders) / sizeof(forbiddenHeaders[0]); ++i)
	{
		if (binSearch(m_headers, (Cgi::Header){BufferView(forbiddenHeaders[0]), BufferView()}) != -1)
		{
			//std::cout << "forbidden header included: " << forbiddenHeaders[0] << std::endl;
			return (mf_setStatus(HeaderData::FAIL, CGI_FAILURE));
		}
	}
	//std::cout << "headers validated" << std::endl;
	return (HeaderData::PASS);
}

HeaderData::Status
HeaderData::mf_parseHeaders(BufferView& view)
{
	//std::cout << "\tBufferView begin: " << view << std::endl;

	size_t pos_LineEnd = view.find(CGI_LINE_SEPARATOR);
	if (pos_LineEnd == BufferView::npos)
	{
		return (HeaderData::NEED_MORE_DATA);
	}
	
	// second newline right at the beginning
	if (pos_LineEnd == 0 && m_state != HeaderData::START)
	{
		m_totalParsedBytes++;
		view = view.substr(pos_LineEnd + 1, view.size() - pos_LineEnd - 1);

		//std::cout << "view size: " << view.size() << std::endl;

		if (mf_validateHeaders() == HeaderData::FAIL)
			return (HeaderData::FAIL);
		//std::cout << "m_hasbody" << m_hasBody << std::endl;
		if (view.size() > 0)
		{
			if (m_hasBody == true)
				m_tempBody = view;
			else
				return (mf_setStatus(HeaderData::FAIL, CGI_FAILURE));
		}
		//std::cout << "new line at beginning, should reach here" << std::endl;
		m_state = HeaderData::FINISH;
		return (HeaderData::PASS);
	}

	m_state = HeaderData::HEADERS;

	BufferView line = view.substr(0, pos_LineEnd);
	//std::cout << "line: '" << line << "', length: " << line.size() << std::endl;
	size_t pos_Separator = line.find(CGI_HEADER_SEPARATOR, 2, 0);
	//std::cout << "pos_Separator: " << pos_Separator << std::endl;

	if (pos_Separator == BufferView::npos)
	{
		//std::cout << "no coma separator found in header" << std::endl;
		return (mf_setStatus(HeaderData::FAIL, CGI_FAILURE));
	}

	BufferView key = line.substr(0, pos_Separator);
	BufferView value = line.substr(pos_Separator + 2, line.size() - pos_Separator - 2);

	if (key == BufferView("Status"))
	{
		if (m_statusCode != -1)
			return (mf_setStatus(HeaderData::FAIL, CGI_FAILURE)); // doubled status header
		m_statusCode = std::atoi(value.data());
		if (m_statusCode != CGI_SUCCESS)
			return (mf_setStatus(HeaderData::FAIL, m_statusCode));
	}
	else	
		m_headers.push_back((Cgi::Header){key, value});	// save header

	//std::cout << "key: '" <<  m_headers.back().key << "', value: '" << m_headers.back().value << "'" << std::endl;
	
	

	m_totalParsedBytes += line.size() + 1;

	view = view.substr(line.size() + 1, view.size() - line.size() - 1); // move view to one past end of line
	
	//std::cout << "\tBufferView end: " << view << std::endl;
	if (view.size() == 0)
		return (HeaderData::NEED_MORE_DATA);
	return (HeaderData::KEEP_PARSING);

	
}

HeaderData::Status	HeaderData::parse(BaseBuffer& buffer)
{
	// pickup where you left off

	BufferView view = BufferView(buffer.data() + m_totalParsedBytes, buffer.size() - m_totalParsedBytes);

	//std::cout << "total parsed bytes: " << m_totalParsedBytes << ", view size: " << view.size() << std::endl;

	if (view.size() == 0)
	{
		if (m_state != HeaderData::FINISH)
			return (mf_setStatus(HeaderData::FAIL, CGI_FAILURE));
		return ((m_statusCode != -1 && m_statusCode != CGI_SUCCESS) ? HeaderData::FAIL : HeaderData::PASS);
	}

	while (view.size() > 0)
	{
		switch ((int)m_state)
		{
			case HeaderData::START: //allow to fall to headers
			case HeaderData::HEADERS:
			{
				HeaderData::Status retStatus = mf_parseHeaders(view);
				if (retStatus != HeaderData::KEEP_PARSING)
					return (retStatus);
				break ;
			}
			case HeaderData::FINISH:
				return (HeaderData::PASS);
		}
	}
	return (HeaderData::NEED_MORE_DATA);
}

HeaderData::Status
HeaderData::mf_setStatus(const HeaderData::Status status, const int statusCode)
{
	m_statusCode = statusCode;
	return (status);
}

const std::vector<Cgi::Header>&
HeaderData::getHeaders() const { return (m_headers); }

int
HeaderData::getStatusCode() const { return (m_statusCode); }

bool
HeaderData::hasBody() const { return (m_hasBody); }

const BufferView&
HeaderData::getTempBody() const { return (m_tempBody); }

HeaderData::State
HeaderData::getState() const { return (m_state); }
