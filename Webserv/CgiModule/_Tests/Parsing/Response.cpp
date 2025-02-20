

# include "Response.hpp"
# include "../../../GenericUtils/Buffer/BaseBuffer.hpp"

# include <cstring>
# include <vector>
# include <algorithm>


# include <iostream>            //DEBUG, erase afterwards

static const char* forbiddenHeaders[] =
{
	"Content-Length",
	"Transfer-Encoding",
	"Connection",
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

namespace Cgi
{
    Response::Response(BaseBuffer& client) 
    : m_state               (Response::HEADERS)
    , m_statusCode          (-1)
    , m_totalParsedBytes    (0)
    , m_client              (client)
    , m_hasBody             (false) {}


    Response::~Response() {}




    Response::Status
    Response::mf_parseBody(BufferView& view)
    {
        (void)view;
        return (Response::PASS);
    }

    Response::Status
    Response::mf_validateHeaders()
    {
        std::sort(m_headers.begin(), m_headers.end(), CompareHeaders());
        
        int m_statusCode= binSearch(m_headers, (Cgi::Header){BufferView("Status"), BufferView()});
        int contentType = binSearch(m_headers, (Cgi::Header){BufferView("Content-Type"), BufferView()});
        int location    = binSearch(m_headers, (Cgi::Header){BufferView("Location"), BufferView()});

        if (m_statusCode == -1 && contentType == -1 && location == -1)
        {
            std::cout << "lacking at least one mandatory header" << std::endl;
            return (Response::FAIL);
        }
        
        if (m_statusCode == -1)
            m_statusCode = CGI_SUCCESS;
        else
        {
            m_statusCode = std::atoi(m_headers[m_statusCode].value.data());
            if (m_statusCode != CGI_SUCCESS)
                return (Response::FAIL);
        }

        if (contentType != -1)
            m_hasBody = true;

        for (size_t i = 0; i < sizeof(forbiddenHeaders) / sizeof(forbiddenHeaders[0]); ++i)
        {
            if (binSearch(m_headers, (Cgi::Header){BufferView(forbiddenHeaders[0]), BufferView()}) != -1)
            {
                std::cout << "forbidden header included: " << forbiddenHeaders[0] << std::endl;
                return (Response::FAIL);
            }
        }
        std::cout << "headers validated" << std::endl;
        return (Response::PASS);
    }

    Response::Status
    Response::mf_parseHeaders(BufferView& view)
    {
        std::cout << "\tBufferView begin: " << view << std::endl;

        size_t pos = view.find(CGI_LINE_SEPARATOR);
        if (pos == BufferView::npos)
        {
            return (Response::NEED_MORE_DATA);
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

        if (binSearch(m_headers, (Cgi::Header){key, value}) != -1)
        {
            std::cout << "duplicate header found: " << key << std::endl;
            return (Response::FAIL);
        }

        m_headers.push_back((Cgi::Header){key, value});	// save header
        std::cout << "key: '" <<  m_headers.back().key << "', value: '" << m_headers.back().value << "'" << std::endl;
        
        m_totalParsedBytes += line.size() + 1;

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
        
        std::cout << "\tBufferView end: " << view << std::endl;
        if (view.size() == 0)
            return (Response::NEED_MORE_DATA);
        return (Response::KEEP_PARSING);
    }

    Response::Status	Response::parse(BaseBuffer& buffer)
    {
        // pickup where you left off
        BufferView view = BufferView(buffer.data() + m_totalParsedBytes, buffer.size() - m_totalParsedBytes);
        std::cout << "total parsed bytes: " << m_totalParsedBytes << ", view size: " << view.size() << std::endl;
        if (view.size() == 0)
        {
            if (m_state == Response::BODY)
            {
                m_state = Response::FINISH;
                return (Response::PASS);
            }
            else
                return (Response::FAIL); // still in headers and received EOF
        }

        while (view.size() > 0)
        {
            switch ((int)m_state)
            {
                case Response::HEADERS:
                {
                    Response::Status retStatus = mf_parseHeaders(view);
                    if (retStatus != Response::KEEP_PARSING)
                        return (retStatus);
                    break ;
                }
                case Response::BODY:
                {
                    if (mf_parseBody(view) == Response::FAIL)
                        return (Response::FAIL);
                    break ;
                }
                case Response::FINISH:
                    return (Response::PASS);
            }
        }
        return (Response::NEED_MORE_DATA);
    }

    const std::vector<Cgi::Header>&
    Response::getHeaders() const { return (m_headers); }

    bool
    Response::hasBody() const { return (m_hasBody); }

    const BufferView&
    Response::getTempBody() const { return (m_tempBody); }
}