


# include "HttpCgiGateway.hpp"
# include "CgiHandlers.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"
# include "../../Connections/Connection/Connection.hpp"

# include <unistd.h> // write

namespace Http
{

    BufferView
    CgiGateway::mf_HttpBodyNone(const BufferView& view)
    {
        // do nothing, return the full view back
        return (view);
    }

    BufferView
    CgiGateway::mf_HttpBodyIgnore(const BufferView& view)
    {
        // return empty view, tells Request "all processed" but does nothing with it
        // we receive and EOF from the request, we are ready to process the response
        
        if (view.size() == 0)
            m_processHttpBody = &CgiGateway::mf_HttpBodyNone;
        return (BufferView());
    }

    BufferView
    CgiGateway::mf_HttpBodySend(const BufferView& view)
    {
		
		if (view.size() == 0)
        {
            m_processHttpBody = &CgiGateway::mf_HttpBodyNone;
			return (BufferView());
        }

		if (!m_canWrite)
			return (view);
		
		int bytesWritten = ::write(m_writeFd, view.data(), view.size());

        // process closed the pipe since we checked, ignore the rest of the body
		if (bytesWritten == -1)
		{
			m_processHttpBody = &CgiGateway::mf_HttpBodyIgnore;
			return (BufferView());
		}

        BufferView remaining = view.substr(bytesWritten, view.size() - bytesWritten);

		return (remaining);        
    }

}