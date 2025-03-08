
#include "../../../HttpResponse/HttpResponse.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

#include "../../../../ServerContext/ServerContext.hpp"
#include "../../../../GenericUtils/Buffer/Buffer.hpp"
#include "../../../../GenericUtils/Buffer/HeapBuffer.hpp"
#include "../../../../GenericUtils/BufferView/BufferView.hpp"
#include "../../../../GenericUtils/StringUtils/StringUtils.hpp"

#include "../RequestResponseUpload/texts.hpp"

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

	try
	{
		Http::RequestData	data;
		Http::Response		response(context);
		
		response.receiveRequestData(data);

		data.multipart_Filename = file1_Name;
		data.status = Http::Status::OK;

		Buffer<5000> fileData;
		Buffer<5000> testBuffer;

		HeapBuffer readBuffer(readBufSize);
		
		fileData.push(file1_Content);

		response.setBuffer_writeFd(testBuffer, Ws::FD_NONE);

        while (fileData.size() && data.status == Http::Status::OK)
        {
            int thisPush = readBuffer.available() < fileData.size() ? readBuffer.available() : fileData.size();
            readBuffer.push(BufferView(fileData.data(), thisPush));
            fileData.truncatePush(BufferView(fileData.data() + thisPush, fileData.size() - thisPush));
            readBuffer.truncatePush(response.receiveRequestBody(readBuffer.view()));
        }

		// manually send EOF
		response.receiveRequestBody(BufferView());
		
		int fd1 = ::open(file1_Name, O_RDONLY);
		EXPECT_EQUAL(fd1 != Ws::FD_NONE, true, "File should be opened");

		testBuffer.read(fd1);
		::close(fd1);
		EXPECT_EQUAL(testBuffer.view(), BufferView(file1_Content, file1_ContentSize), "File content should match");

		/////////

		data.multipart_Filename = file2_Name;
		fileData.truncatePush(file2_Content);

		while (fileData.size() && data.status == Http::Status::OK)
		{
			int thisPush = readBuffer.available() < fileData.size() ? readBuffer.available() : fileData.size();
			readBuffer.push(BufferView(fileData.data(), thisPush));
			fileData.truncatePush(BufferView(fileData.data() + thisPush, fileData.size() - thisPush));
			readBuffer.truncatePush(response.receiveRequestBody(readBuffer.view()));
		}

		// manually send EOF
		response.receiveRequestBody(BufferView());

		fd1 = ::open(file1_Name, O_RDONLY);
		testBuffer.read(fd1);
		::close(fd1);
		EXPECT_EQUAL(testBuffer.view(), BufferView(file1_Content, file1_ContentSize), "File content should match");

		int fd2 = ::open(file2_Name, O_RDONLY);
		testBuffer.read(fd2);
		::close(fd2);
		EXPECT_EQUAL(testBuffer.view(), BufferView(file2_Content, file2_ContentSize), "File content should match");


		TEST_PASSED_MSG("FileUpload test passed, read buffer size: " + TestHelpers::to_string(readBufSize));
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
}



int main(void)
{

	TEST_HEADER("HttpResponse - File Upload");

	int testNumber = 1;

	for (size_t i = 10; i < 500; i += 20)
		fileUpload(testNumber, i);

	TEST_FOOTER;

	::unlink(file1_Name);
	::unlink(file2_Name);

	return (0);
}
