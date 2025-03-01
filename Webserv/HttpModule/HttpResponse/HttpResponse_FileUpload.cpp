

# include "HttpResponse.hpp"

namespace Http
{
	BufferView
	Response::mf_processBodyNone(const BufferView& view)
	{
		// does nothing, returns the full view back
		return (view);
	}

	BufferView
	Response::mf_processBodyIgnore(const BufferView& view)
	{
		//returns empty view, tells Request "all processed" but does nothing with it
		(void)view;

		return (BufferView());
	}

	BufferView
	Response::mf_processBodyUpload(const BufferView& view)
	{
		BufferView		remaining;
		int 			bytesWritten = 0;

		// receive empty view, queue to EOF
		if (view.size() == 0
		|| m_responseData.requestData->multipart_Filename.empty())
		{
			m_file.close();

			// CHECK FOR FAILURE IN EXECUTION NEEDED, UNLINK FILE
			
			return (view);
		}

		if (m_responseData.requestData->multipart_Filename != m_file.path())
		{
			m_file.close();
			if (!m_file.open(m_responseData.requestData->multipart_Filename.c_str(),
				O_CREAT | O_RDWR | O_TRUNC, 0666))
				goto exitFailure;
		}

		bytesWritten = m_file.write(view.data(), view.size());
		if (bytesWritten < 0)
			goto exitFailure;
		
		remaining = view.substr(bytesWritten, view.size() - bytesWritten);

		return (remaining);
	
	exitFailure:
		m_responseData.requestStatus = Http::Status::INTERNAL_ERROR;
		m_processFunction = &Response::mf_processBodyNone;
		m_fillFunction = &Response::mf_fillErrorResponse;
		return (view);
	}


	BufferView
	Response::mf_processBodyCgi(const BufferView& view)
	{
		// does nothing, returns the full view back
		return (view);
	}
}