


# include "HttpCgiResponse.hpp"
# include "CgiHandlers.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"
# include "../../Connections/Connection/Connection.hpp"

# include <unistd.h> // write

namespace Http
{

    BufferView
    CgiResponse::mf_HttpBodyNone(const BufferView& view)
    {
        // do nothing, return the full view back
        return (view);
    }

    BufferView
    CgiResponse::mf_HttpBodyIgnore(const BufferView& view)
    {
        // return empty view, tells Request "all processed" but does nothing with it
        // we receive and EOF from the request, we are ready to process the response
        
        if (view.size() == 0)
            m_processHttpBody = &CgiResponse::mf_HttpBodyNone;
        return (BufferView());
    }

    BufferView
    CgiResponse::mf_HttpBodySend(const BufferView& view)
    {
		
		if (view.size() == 0)
        {
            //std::cout << "received EOF, sent all" << std::endl;
            m_processHttpBody = &CgiResponse::mf_HttpBodyNone;
			return (BufferView());
        }

		if (!m_canWrite)
			return (view);
		
        // received fd_none, write closed, ignore the rest of the body
        if (m_writeFd == Ws::FD_NONE)
        {
            //std::cout << "write closed, nothieng to send" << std::endl;
            m_processHttpBody = &CgiResponse::mf_HttpBodyIgnore;
			return (BufferView());
        }

		int bytesWritten = ::write(m_writeFd, view.data(), view.size());

        // process closed the pipe since we checked, ignore the rest of the body
		if (bytesWritten == -1)
		{
			m_processHttpBody = &CgiResponse::mf_HttpBodyIgnore;
			return (BufferView());
		}

        BufferView remaining = view.substr(bytesWritten, view.size() - bytesWritten);

		return (remaining);        
    }

}