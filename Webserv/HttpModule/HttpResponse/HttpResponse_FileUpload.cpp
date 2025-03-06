

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
			if (!m_responseData.requestData->multipart_Filename.empty())
			{
				//std::cout << "\t\t\t\t closing file" << std::endl;
				m_file.close();
			}
			else if (m_responseData.requestData->multipart_Name.empty())
			{
				//std::cout << "\t\t\t\t ready to respond" << std::endl;
				// finished
				m_fillFunction = &Response::mf_fillResponseLine;
				m_processFunction = &Response::mf_processBodyIgnore;
				mf_prepareErrorMessage();
				m_fillFunctionBody = &Response::mf_fillDefaultPage;
				m_responseData.requestStatus = Http::Status::OK;
			}
			return (BufferView());
		}

		if (m_responseData.requestData->multipart_Filename.empty())
		{
			//std::cout << "form data, ignore" << std::endl;
			return (BufferView());
		}

		if (m_responseData.requestData->multipart_Filename != m_file.path())
		{
			//std::cout << "new file, open" << std::endl;
			// new file, open
			if (!m_file.open(m_responseData.requestData->multipart_Filename.c_str(),
				O_CREAT | O_RDWR | O_TRUNC, 0666))
				goto exitFailure;
		}

		bytesWritten = m_file.write(view.data(), view.size());
		//std::cout << "view.size() = " << view.size() << ", bytesWritten = " << bytesWritten << std::endl;
		if (bytesWritten < 0)
			goto exitFailure;
		
		remaining = view.substr(bytesWritten, view.size() - bytesWritten);

		return (remaining);
	
	exitFailure:
		std::cout << "error writing file" << std::endl;
		m_responseData.requestStatus = Http::Status::INTERNAL_ERROR;
		m_processFunction = &Response::mf_processBodyIgnore;
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
