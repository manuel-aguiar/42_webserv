

# include "HttpCgiGateway.hpp"
# include "CgiHandlers.hpp"
# include "../../GenericUtils/Buffer/BaseBuffer.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../CgiModule/HeaderData/HeaderData.hpp"
# include "../../Connections/Connection/Connection.hpp"

# include <unistd.h> // write


namespace Http
{

	// must send everything before starting to write to client
	// therefore, try to send request body full until receiving an empty view (EOF)
	//           -> engage the fill function pointers
	// if script close the write pipe, turn into send body ignore
	// ignore any data that comes in, once receiving an EOF, engage the
	//           -> engage the fill function pointers

	BufferView
	CgiGateway::sendHttpBody(const BufferView& view)
	{
		return ((this->*m_processHttpBody)(view));
	}


	Http::ResponseStatus::Type
	CgiGateway::fillWriteBuffer(BaseBuffer& writeBuffer)
	{
		// still processing body, can't start writing the response yet
		if (m_processHttpBody != &CgiGateway::mf_HttpBodyNone)
			return (Http::ResponseStatus::WAITING);
		return ((this->*m_fillFunction)(writeBuffer));
	}

}