

# include "HttpResponse.hpp"

namespace Http
{
	BufferView
	Response::mf_processBodyNone(const BufferView& view)
	{
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
		
		
		if (view.size() == 0) // eof
		{
			//std::cout << "received end of file" << std::endl;
			if (m_responseData.requestData->multipart_Filename.empty())
			{
				//std::cout << "no filename" << std::endl;
				///both empty means end
				if (m_responseData.requestData->multipart_Name.empty()) 
				{
					//std::cout << "nor field name" << std::endl;
					m_fillFunction = &Response::mf_fillResponseLine;
					m_processFunction = &Response::mf_processBodyIgnore;
					mf_prepareErrorMessage();
					m_fillFunctionBody = &Response::mf_fillDefaultPage;
					m_responseData.requestStatus = Http::Status::OK;
				}

				return (BufferView());
			}
			m_file.close();
			return (BufferView());
		}

		if (m_responseData.requestData->multipart_Filename.empty())
		{
			// form data, ignore
			return (BufferView());
		}

		if (m_responseData.requestData->multipart_Filename != m_file.path())
		{
			// new file, open
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
		m_defaultPageContent = mf_generateDefaultErrorPage(m_responseData.requestStatus, "Implement Me (this is hardcoded)");
		m_fillFunction = &Response::mf_fillDefaultPage;
		return (view);
	}


	BufferView
	Response::mf_processBodyCgi(const BufferView& view)
	{
		ASSERT_EQUAL(m_cgiResponse != NULL, true, "Response: CgiResponse not set");
		return (m_cgiResponse->sendHttpBody(view));
	}
}