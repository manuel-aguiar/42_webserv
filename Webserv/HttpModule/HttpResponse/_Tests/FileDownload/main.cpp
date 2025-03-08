
#include "../../../HttpResponse/HttpResponse.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

#include "../../../../ServerContext/ServerContext.hpp"
#include "../../../../GenericUtils/Buffer/Buffer.hpp"
#include "../../../../GenericUtils/Buffer/HeapBuffer.hpp"
#include "../../../../GenericUtils/BufferView/BufferView.hpp"
#include "../../../../GenericUtils/StringUtils/StringUtils.hpp"

#include "../RequestResponseUpload/texts.hpp"

///////////////////////////////////////////////////////////

static const char* file1_Name = "base.txt";
static const char* file1_Content = potato_text;
static const size_t file1_ContentSize = potato_text_size;


void fileDownload(int& testNumber, const BaseBuffer& comparison)
{
	TEST_INTRO(testNumber++);
	ServerContext context;

	try
	{
		Http::RequestData data;
		Http::Response response(context);
		
		
		data.path = "base.txt";
		data.status = Http::Status::OK;
		
		response.receiveRequestData(data);

		HeapBuffer testBuffer(comparison.capacity() + 500);
		response.setBuffer_writeFd(testBuffer, Ws::FD_NONE);
		
		Http::IOStatus::Type status = Http::IOStatus::WAITING;
        while (status != Http::IOStatus::FINISHED)
			status = response.fillWriteBuffer(testBuffer);

		EXPECT_EQUAL(testBuffer.view(), comparison.view(), "File content should match");

		TEST_PASSED_MSG("FileUpload test passed, read buffer size: ");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
}



int main(void)
{

	TEST_HEADER("HttpResponse - File Download");

	int testNumber = 1;

	int fd = ::open(file1_Name, O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (fd == -1)
	{
		std::cerr << "Failed to open file: " << file1_Name << std::endl;
		return (1);
	}

	::write(fd, file1_Content, file1_ContentSize);
	::close(fd);

	fd = ::open(file1_Name, O_RDONLY, 0666);
	if (fd == -1)
	{
		std::cerr << "Failed to open file: " << file1_Name << std::endl;
		return (1);
	}
	struct stat buffer;

	if (stat(file1_Name, &buffer) != 0)
	{
		std::cerr << "Failed to stat file: " << file1_Name << std::endl;
		return (1);
	}

	HeapBuffer comparison(buffer.st_size);
	comparison.read(fd);

	fileDownload(testNumber, comparison);

	TEST_FOOTER;

	return (0);
}
