#include "../../HttpRequest.hpp"
#include "../../../HttpResponse/HttpResponse.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"
#include "../../../../GenericUtils/Buffer/Buffer.hpp"
#include "../../../../GenericUtils/Buffer/HeapBuffer.hpp"
#include "../../../../GenericUtils/BufferView/BufferView.hpp"
#include "../../../../GenericUtils/StringUtils/StringUtils.hpp"

#include <sstream>

// Mock Http::Response will drop the message body here
extern std::map<std::string, std::string > g_mockMsgBody;

void    chunkedReadBuffer(int& testNumber, size_t readBufSize)
{
    TEST_INTRO(testNumber++);

    ServerContext context;

    Http::Request   HttpRequest(context);
    Http::Response  response(context);
    const Http::RequestData& requestData = HttpRequest.getData();

    Buffer<1024> bufferRequest;

    HttpRequest.setResponse(response);

    HeapBuffer buffer(readBufSize);

    std::string requestHeader = 
    "POST /upload HTTP/1.1\r\n"
    "Host: example.com\r\n"
    "Content-Type: multipart/form-data; boundary=----WebKitFormBoundary12345\r\n"
    "Content-Length: 500\r\n"  // Placeholder, must be calculated
    "\r\n";

    std::string requestBodyMultipart = 
        "------WebKitFormBoundary12345\r\n"
        "Content-Disposition: form-data; name=\"file1\"; filename=\"file1.txt\"\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "This is file 1.\r\n"
        "------WebKitFormBoundary12345\r\n"
        "Content-Disposition: form-data; name=\"file2\"; filename=\"file2.txt\"\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "This is file 2.\r\n"
        "------WebKitFormBoundary12345--\r\n";

    std::string requestBodyTranslated = "Wikipedia in \rchunks.\r";

    std::string request = requestHeader + requestBodyMultipart;

    bufferRequest.push(request.c_str(), request.size());

    g_mockMsgBody.clear();

    try
    {
        while (bufferRequest.size() && requestData.status == Http::Status::OK)
        {
            int thisPush = buffer.available() < bufferRequest.size() ? buffer.available() : bufferRequest.size();
            buffer.push(BufferView(bufferRequest.data(), thisPush));
            bufferRequest.truncatePush(BufferView(bufferRequest.data() + thisPush, bufferRequest.size() - thisPush));

            // parse, tell the buffer to put the unconsumed part at the beginning
            buffer.truncatePush(HttpRequest.parse(buffer));

            std::cout << "loop requestData.status = " << requestData.status << std::endl;
        }
        std::cout << "requestData.status = " << requestData.status << std::endl;
        EXPECT_EQUAL(BufferView(g_mockMsgBody["file1.txt"]), BufferView("This is file 1."), "Body should match");
        EXPECT_EQUAL(BufferView(g_mockMsgBody["file2.txt"]), BufferView("This is file 2."), "Body should match");

        TEST_PASSED_MSG(std::string("Valid body, chuncked, weird test with carriage return in body, readBuf size: ") 
        + TestHelpers::to_string(readBufSize)  + " '" + requestBodyTranslated + "'");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
}


int main()
{
    TEST_HEADER("Http Request - Chunked Body");
    int testNumber = 1;
    chunkedReadBuffer(testNumber, 100);
    for (size_t i = 75; i < 123; ++i)
        chunkedReadBuffer(testNumber, i);
    chunkedReadBuffer(testNumber, 1024);
    TEST_FOOTER;
    return 0;
}
