
#include "../../../HttpResponse/HttpResponse.hpp"
#include "../../../HttpRequest/HttpRequest.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

#include "../../../../ServerContext/ServerContext.hpp"
#include "../../../../GenericUtils/Buffer/Buffer.hpp"
#include "../../../../GenericUtils/Buffer/HeapBuffer.hpp"
#include "../../../../GenericUtils/BufferView/BufferView.hpp"
#include "../../../../GenericUtils/StringUtils/StringUtils.hpp"

# include "texts.cpp"

extern const char potato_text[];
extern const char orange_text[];
extern const size_t potato_text_size;
extern const size_t orange_text_size;

///////////////////////////////////////////////////////////

static const char* file1_Name = "test1.txt";
static const char* file1_Content = potato_text;
static const size_t file1_ContentSize = potato_text_size;

static const char* file2_Name = "test2.txt";
static const char* file2_Content = orange_text;
static const size_t file2_ContentSize = orange_text_size;

void fileUpload(int& testNumber, size_t readBufSize)
{
	TEST_INTRO(testNumber++);
	ServerContext context;

	::unlink(file1_Name);
	::unlink(file2_Name);

	try
	{
		Http::Response 		response(context);
		Http::Request 		request(context);

		request.setResponse(response);
		response.setRequest(request);

		///////////////////

		// manually send the request
		std::string boundary = "----WebKitFormBoundary12345";
		std::string contentDisp1 = "Content-Disposition: form-data; name=\"file1\"; filename=\"" + std::string(file1_Name) + "\"";
		std::string contentDisp2 = "Content-Disposition: form-data; name=\"file2\"; filename=\"" + std::string(file2_Name) + "\"";
		std::string form1 = "Content-Disposition: form-data; name=\"field1\";";
		std::string form2 = "Content-Disposition: form-data; name=\"field2\";";

		std::string requestBodyMultipart = 

			"--" + boundary + "\r\n"
			///////////////////////  a file
			+ contentDisp1 + "\r\n"
			"\r\n"
			+ file1_Content + "\r\n"
			"--" + boundary + "\r\n"
			///////////////////////  same file twice
			+ contentDisp1 + "\r\n"
			"\r\n"
			+ file1_Content + "\r\n"
			"--" + boundary + "\r\n"
			////////////////////////  form 1
			+ form1 + "\r\n"
			"\r\n"
			"--" + boundary + "\r\n"
			////////////////////////  form 2
			+ form2 + "\r\n"
			"\r\n"
			"--" + boundary + "\r\n"
			/////////////////////// second file
			+ contentDisp2 + "\r\n"
			"\r\n"
			+ file2_Content + "\r\n"
			"--" + boundary + "\r\n"
			//////////////////////// form2 again
			+ form2 + "\r\n"
			"\r\n"
			"--" + boundary + "\r\n"
			///////////////////////// second file again
			+ contentDisp2 + "\r\n"
			"\r\n"
			+ file2_Content + "\r\n"
			"--" + boundary 
			//////////////////////// the end
			+ "--\r\n" + "\r\n";


		std::string requestHeader = 
		"POST /upload HTTP/1.1\r\n"
		"hoSt: example.com\r\n"
		"ConTEnt-Type: multipart/form-data; boundary=" + boundary + "\r\n"
		"contenT-lenGth: " + TestHelpers::to_string(requestBodyMultipart.size()) + "\r\n"
		"\r\n";
	
		std::string fullRequest = requestHeader + requestBodyMultipart;

		///////////////

		HeapBuffer bufferRequest(fullRequest.size() + 100);
		HeapBuffer readBuffer(readBufSize);
		HeapBuffer testBuffer(std::max(file1_ContentSize, file2_ContentSize) + 100);
		
		bufferRequest.push(fullRequest.c_str(), fullRequest.size());

		request.setBuffer_ReadFd(readBuffer, Ws::FD_NONE);

        while (bufferRequest.size() && request.getStatus() == Http::Status::OK)
        {
			int thisPush = readBuffer.available() < bufferRequest.size() ? readBuffer.available() : bufferRequest.size();
            readBuffer.push(BufferView(bufferRequest.data(), thisPush));
            bufferRequest.truncatePush(BufferView(bufferRequest.data() + thisPush, bufferRequest.size() - thisPush));
            request.parse();
        }

		int fd1 = ::open(file1_Name, O_RDONLY);
		testBuffer.read(fd1);
		::close(fd1);

		EXPECT_EQUAL(testBuffer.view(), BufferView(file1_Content, file1_ContentSize), "File content should match");

		int fd2 = ::open(file2_Name, O_RDONLY);
		testBuffer.read(fd2);
		::close(fd2);
		EXPECT_EQUAL(testBuffer.view(), BufferView(file2_Content, file2_ContentSize), "File content should match");

		EXPECT_EQUAL(request.getStatus(), Http::Status::OK, "Request status should be OK");
		EXPECT_EQUAL(request.getParsingState(), Http::Request::COMPLETED, "Request parsing state should be BODY_DONE");

		TEST_PASSED_MSG("FileUpload Request-Response integration test passed, read buffer size: " + TestHelpers::to_string(readBufSize));
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
		exit(0);
	}

	::unlink(file1_Name);
	::unlink(file2_Name);

}



int main(void)
{

	TEST_HEADER("Request to Response - File Upload");



	int testNumber = 1;

	//fileUpload(testNumber, 400);
	for (size_t i = 200; i < 500; i += 1)
		fileUpload(testNumber, i);

	TEST_FOOTER;

	return (0);
}