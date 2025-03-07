

#include "HttpResponse.hpp"
#include "../../ServerConfig/ServerLocation/ServerLocation.hpp"
#include "../../ServerConfig/ServerBlock/ServerBlock.hpp"

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
		std::cout << "---mf_processBodyUpload-" << std::endl;
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
				std::cout << "file uploaded " << m_responseData.targetPath << std::endl;
			}
			return (BufferView());
		}

		if (m_responseData.requestData->multipart_Filename.empty())
		{
			//std::cout << "form data, ignore" << std::endl;
			return (BufferView());
		}

		std::string fullPath;
		
		if (BufferView(m_responseData.requestData->multipart_Filename) != m_file.name())
		{
			ASSERT_EQUAL(m_responseData.serverLocation != NULL, true, "Response: ServerLocation not set");
			ASSERT_EQUAL(m_responseData.serverBlock != NULL, true, "Response: ServerBlock not set");

			if (!m_responseData.serverLocation->getUploadPath().empty())
			{
				fullPath = m_responseData.serverLocation->getUploadPath()
							+ "/"
							+ m_responseData.requestData->multipart_Filename;
			}
			else
			{
				std::cout << "uploading file to " << m_responseData.targetPath << std::endl;
				if (!m_responseData.serverLocation->getRoot().empty())
				{
					fullPath = m_responseData.serverLocation->getRoot()
								+ "/" 
								+ m_responseData.requestData->multipart_Filename;
				}
				else
				{
					fullPath = m_responseData.serverBlock->getRoot()
								+ "/"
								+ m_responseData.requestData->multipart_Filename;
				}
			}
			if (!m_file.open(fullPath.c_str(),
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
		m_responseData.errorMessage = "Upload failed";
		m_defaultPageContent = mf_generateDefaultErrorPage(m_responseData.requestStatus, m_responseData.errorMessage);
		m_processFunction = &Response::mf_processBodyIgnore;
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
