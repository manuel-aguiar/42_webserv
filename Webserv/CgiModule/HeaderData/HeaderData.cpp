

# include "HeaderData.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"

# include <cstring>
# include <vector>
# include <algorithm>


# include <iostream>            //DEBUG, erase afterwards

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

void	toLowerCase(BufferView& view)
{
	char *data = const_cast<char*>(view.data());

	for (size_t i = 0; i < view.size(); ++i)
	{
		if (data[i] >= 'A' && data[i] <= 'Z')
			data[i] += 32;
	}
}

namespace Cgi
{
HeaderData::HeaderData() 
	: m_state               (HeaderData::START)
	, m_statusCode          (-1)
	, m_totalParsedBytes    (0)
	, m_lastBufferSize      (0)
	, m_findPivot		   	(0)
	, m_hasBody             (false) {}


HeaderData::~HeaderData() {}



void
HeaderData::reset()
{
	m_state = HeaderData::START;
	m_statusCode = -1;
	m_totalParsedBytes = 0;
	m_lastBufferSize = 0;
	m_findPivot = 0;
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

	int contentType = binSearch(m_headers, Cgi::Header("content-type", ""));
	int location    = binSearch(m_headers, Cgi::Header("location", ""));

	if (m_statusCode == -1 && contentType == -1 && location == -1)
	{
		//std::cout << "lacking at least one mandatory header" << std::endl;
		return (mf_setStatus(HeaderData::FAIL, CGI_FAILURE));
	}
	
	if (m_statusCode == -1)
		m_statusCode = CGI_SUCCESS;

	if (contentType != -1)
		m_hasBody = true;

	return (HeaderData::PASS);
}

HeaderData::Status
HeaderData::mf_parseHeaders(BufferView& receivedView)
{
	BufferView remaining = receivedView;
	const BufferView CgiDelimiter = BufferView(CGI_LINE_SEPARATOR, std::strlen(CGI_LINE_SEPARATOR));
	const BufferView headedSeparator = BufferView(CGI_HEADER_SEPARATOR, std::strlen(CGI_HEADER_SEPARATOR));

	

	if (remaining.size() == 0)
		return (HeaderData::NEED_MORE_DATA);

	while (remaining.size() > 0)
	{	
		//std::cout << "remaining: " << remaining << std::endl;
		size_t pos_LineEnd = remaining.find(CgiDelimiter, m_findPivot);
		if (pos_LineEnd == BufferView::npos)
		{
			m_findPivot = std::max((int)remaining.size() - (int)CgiDelimiter.size(), 0);
			// if bigger than buffer capacity, fail
			return (HeaderData::NEED_MORE_DATA);
		}

		m_findPivot = 0;
		
		// second newline right at the beginning
		if ((pos_LineEnd == 0 || (pos_LineEnd == 1 && remaining[0] == '\r'))
		&& m_state != HeaderData::START)
		{
			//std::cout << "\t\t\t exiting heaer parsing" << std::endl;
			m_totalParsedBytes += CgiDelimiter.size();
			remaining = remaining.substr(pos_LineEnd + CgiDelimiter.size(), remaining.size() - pos_LineEnd - CgiDelimiter.size());
	
			if (mf_validateHeaders() == HeaderData::FAIL)
			{
				//std::cout << "header validation failed" << std::endl;
				return (HeaderData::FAIL);
			}

			if (remaining.size() > 0)
			{
				if (m_hasBody == true)
					m_tempBody = remaining;
				else
					return (mf_setStatus(HeaderData::FAIL, CGI_FAILURE));
			}
			//std::cout << "new line at beginning, should reach here" << std::endl;
			m_state = HeaderData::FINISH;
			return (HeaderData::PASS);
		}
	
		m_state = HeaderData::HEADERS;
	
		BufferView line = remaining.substr(0, pos_LineEnd);

		// trim, php-cgi already output \r\n........... not CGI spec, requires just \n
		bool isHttpTerminated = false;
		if (line[line.size() - 1] == '\r')
		{
			isHttpTerminated = true;
			line = line.substr(0, line.size() - 1);
		}

		//std::cout << "line:\t\t\t\t '" << line << "', length: " << line.size() << std::endl;
		size_t pos_Separator = line.find(headedSeparator, 0);
		//std::cout << "pos_Separator: " << pos_Separator << std::endl;
	
		if (pos_Separator == BufferView::npos)
		{
			//std::cout << "no coma separator found in header" << std::endl;
			return (mf_setStatus(HeaderData::FAIL, CGI_FAILURE));
		}
	
		BufferView key = line.substr(0, pos_Separator);
		BufferView value = line.substr(pos_Separator + headedSeparator.size(), line.size() - pos_Separator - headedSeparator.size());
		
		toLowerCase(key);

		//std::cout << "key:\t\t\t\t '" << key << "', length: " << key.size() << std::endl;
		//std::cout << "value:\t\t\t\t '" << value << "', length: " << value.size() << std::endl;

		if (key == BufferView("status"))
		{
			if (m_statusCode != -1)
				return (mf_setStatus(HeaderData::FAIL, CGI_FAILURE)); // doubled status header
			m_statusCode = std::atoi(value.data());
			if (m_statusCode != CGI_SUCCESS)
				return (mf_setStatus(HeaderData::FAIL, m_statusCode));
		}
		else	
			m_headers.push_back(Cgi::Header(key, value));	// save header
		
		size_t forward = line.size() + CgiDelimiter.size() + isHttpTerminated;
		m_totalParsedBytes += forward;
		remaining = remaining.substr(forward, remaining.size() - forward); // move view to one past end of line
	}

	return (HeaderData::NEED_MORE_DATA);
}

HeaderData::Status	HeaderData::parse(BaseBuffer& buffer)
{
	BufferView view = BufferView(buffer.data() + m_totalParsedBytes, buffer.size() - m_totalParsedBytes);

	//std::cout << view << std::endl;

	if (view.size() == 0 || m_lastBufferSize == (int)buffer.size())
	{
		//std::cout << "no new data" << std::endl;
		if (m_state != HeaderData::FINISH)
			return (mf_setStatus(HeaderData::FAIL, CGI_FAILURE));
		return ((m_statusCode != -1 && m_statusCode != CGI_SUCCESS) ? HeaderData::FAIL : HeaderData::PASS);
	}

	m_lastBufferSize = buffer.size();

	HeaderData::Status retStatus = mf_parseHeaders(view);
	//std::cout << "retStatus: " << retStatus << std::endl;
	if (retStatus != HeaderData::NEED_MORE_DATA)
		return (retStatus);

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

HeaderData::ParsingState
HeaderData::getParsingState() const { return (m_state); }

}
