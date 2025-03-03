

# include "HeaderData.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"

# include <cstring>
# include <vector>
# include <algorithm>

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
			return (mf_setStatus(HeaderData::FAIL, Cgi::RequestConsts::Status::FAILURE));
		}
	}

	int contentType = binSearch(m_headers, Cgi::Header(Cgi::RequestConsts::Header::ContentType, ""));
	int location    = binSearch(m_headers, Cgi::Header(Cgi::RequestConsts::Header::Location, ""));

	if (m_statusCode == -1 && contentType == -1 && location == -1)
	{
		//std::cout << "lacking at least one mandatory header" << std::endl;
		return (mf_setStatus(HeaderData::FAIL, Cgi::RequestConsts::Status::FAILURE));
	}
	
	if (m_statusCode == -1)
		m_statusCode = Cgi::RequestConsts::Status::SUCCESS;

	if (contentType != -1)
		m_hasBody = true;

	return (HeaderData::PASS);
}

HeaderData::Status
HeaderData::mf_parseHeaders(BufferView& receivedView)
{

	BufferView remaining = receivedView;
	const BufferView CgiDelimiter = BufferView(Cgi::RequestConsts::Separator::Line, std::strlen(Cgi::RequestConsts::Separator::Line));
	const BufferView headerSeparator = BufferView(Cgi::RequestConsts::Separator::Header, std::strlen(Cgi::RequestConsts::Separator::Header));

	

	if (remaining.size() == 0)
		return (HeaderData::NEED_MORE_DATA);

	while (remaining.size() > 0)
	{	
		size_t pos_LineEnd = remaining.find(CgiDelimiter, m_findPivot);
		if (pos_LineEnd == BufferView::npos)
		{
			m_findPivot = std::max((int)remaining.size() - (int)CgiDelimiter.size(), 0);
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
					return (mf_setStatus(HeaderData::FAIL, Cgi::RequestConsts::Status::FAILURE));
			}
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

		size_t pos_Separator = line.find(headerSeparator, 0);
	
		if (pos_Separator == BufferView::npos)
			return (mf_setStatus(HeaderData::FAIL, Cgi::RequestConsts::Status::FAILURE));
	
		BufferView key = line.substr(0, pos_Separator).trim(" \t\v\n\r").modify_ToCapitalized();
		BufferView value = line.substr(pos_Separator + headerSeparator.size(), line.size() - pos_Separator - headerSeparator.size()).trim(" \t\v\n\r");
		
		//std::cout << "key:\t\t\t\t '" << key << "', length: " << key.size() << std::endl;
		//std::cout << "value:\t\t\t\t '" << value << "', length: " << value.size() << std::endl;
		if (key == BufferView(Cgi::RequestConsts::Header::Status))
		{
			if (m_statusCode != -1)
				return (mf_setStatus(HeaderData::FAIL, Cgi::RequestConsts::Status::FAILURE)); // doubled status header
			m_statusCode = std::atoi(value.data());
			if (m_statusCode != Cgi::RequestConsts::Status::SUCCESS)
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


	if (view.size() == 0 || m_lastBufferSize == (int)buffer.size())
	{
		if (m_state != HeaderData::FINISH)
			return (mf_setStatus(HeaderData::FAIL, Cgi::RequestConsts::Status::FAILURE));
		return ((m_statusCode != -1 && m_statusCode != Cgi::RequestConsts::Status::SUCCESS) ? HeaderData::FAIL : HeaderData::PASS);
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
