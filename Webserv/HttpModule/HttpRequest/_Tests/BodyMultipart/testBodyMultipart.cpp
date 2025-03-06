#include "../../HttpRequest.hpp"
#include "../../../HttpResponse/HttpResponse.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"
#include "../../../../ServerContext/ServerContext.hpp"
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
    Http::Response  HttpResponse(context);
    const Http::RequestData& requestData = HttpRequest.getData();

    Buffer<2048> bufferRequest;

    HttpRequest.setResponse(HttpResponse);
    HttpResponse.setRequest(HttpRequest);

    HeapBuffer buffer(readBufSize);
    HttpRequest.setBuffer_ReadFd(buffer, Ws::FD_NONE);

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
        "------WebKitFormBoundary12345\r\n"
        "Content-Disposition: form-data; name=\"file3\"; filename=\"file3.txt\"\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "This is file 3.\r\n"
        "------WebKitFormBoundary12345\r\n"
        "Content-Disposition: form-data; name=\"file4\"; filename=\"file4.txt\"\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "This is file 4.\r\n"
        "------WebKitFormBoundary12345\r\n"
        "Content-Disposition: form-data; name=\"file4\"\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "------WebKitFormBoundary12345\r\n"
        "Content-Disposition: form-data; name=\"file5\"; filename=\"file5.txt\"\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "This is file 5.\r\n"        
        "------WebKitFormBoundary12345--\r\n"
        ;

        std::string requestHeader = 
        "POST /upload HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Content-Type: multipart/form-data; boundary=----WebKitFormBoundary12345\r\n"
        "Content-Length: " + TestHelpers::to_string(requestBodyMultipart.size()) + "\r\n"  // Placeholder, must be calculated
        "\r\n";

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
            HttpRequest.parse();

            //std::cout << "loop requestData.status = " << requestData.status << std::endl;
        }
        //std::cout << "requestData.status = " << requestData.status << std::endl;
        EXPECT_EQUAL(BufferView(g_mockMsgBody["file1.txt"]), BufferView("This is file 1."), "Body should match");
        EXPECT_EQUAL(BufferView(g_mockMsgBody["file2.txt"]), BufferView("This is file 2."), "Body should match");
        EXPECT_EQUAL(BufferView(g_mockMsgBody["file3.txt"]), BufferView("This is file 3."), "Body should match");
        EXPECT_EQUAL(BufferView(g_mockMsgBody["file4.txt"]), BufferView("This is file 4."), "Body should match");
        EXPECT_EQUAL(BufferView(g_mockMsgBody["file5.txt"]), BufferView("This is file 5."), "Body should match");
        EXPECT_EQUAL(requestData.status, Http::Status::OK, "Request should be OK");
        EXPECT_EQUAL(HttpRequest.getParsingState(), Http::Request::COMPLETED, "request should be completed by now");

        TEST_PASSED_MSG(std::string("Valid body, multipart, file1.txt [" 
        + g_mockMsgBody["file1.txt"] + "], file2.txt [" 
        + g_mockMsgBody["file2.txt"] + "], readBuf size: ") 
        + TestHelpers::to_string(readBufSize));
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
}


void    chunkedReadBuffer2(int& testNumber, size_t readBufSize)
{
    TEST_INTRO(testNumber++);

    ServerContext context;

    Http::Request   HttpRequest(context);
    Http::Response  HttpResponse(context);
    const Http::RequestData& requestData = HttpRequest.getData();

    Buffer<2048> bufferRequest;

    HttpRequest.setResponse(HttpResponse);
    HttpResponse.setRequest(HttpRequest);

    HeapBuffer buffer(readBufSize);
    HttpRequest.setBuffer_ReadFd(buffer, Ws::FD_NONE);

    const char* requestBodyMultipart =
    "POST /post/ HTTP/1.1\r\n"
    "Host: localhost:8080\r\n"
    "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:135.0) Gecko/20100101 Firefox/135.0\r\n"
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
    "Accept-Language: en-US,en;q=0.5\r\n"
    "Accept-Encoding: gzip, deflate, br, zstd\r\n"
    "Content-Type: multipart/form-data; boundary=----geckoformboundaryef89c3853670c9dfbafd72221d637292\r\n"
    "Content-Length: 333\r\n"
    "Origin: http://localhost:8080\r\n"
    "Connection: keep-alive\r\n"
    "Referer: http://localhost:8080/upload.html\r\n"
    "Upgrade-Insecure-Requests: 1\r\n"
    "Sec-Fetch-Dest: document\r\n"
    "Sec-Fetch-Mode: navigate\r\n"
    "Sec-Fetch-Site: same-origin\r\n"
    "Sec-Fetch-User: ?1\r\n"
    "Priority: u=0, i\r\n"
    "\r\n"
    "------geckoformboundaryef89c3853670c9dfbafd72221d637292\r\n"
    "Content-Disposition: form-data; name=\"file\"; filename=\"small.text\"\r\n"
    "Content-Type: text/plain\r\n"
    "\r\n"
    "small text file\r\n"
    "------geckoformboundaryef89c3853670c9dfbafd72221d637292\r\n"
    "Content-Disposition: form-data; name=\"text\"\r\n"
    "\r\n"
    "------geckoformboundaryef89c3853670c9dfbafd72221d637292--\r\n"
    ;
    
    std::string requestBodyTranslated = "Wikipedia in \rchunks.\r";

    std::string request = requestBodyMultipart;

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
            HttpRequest.parse();

            //std::cout << "loop requestData.status = " << requestData.status << std::endl;
        }
        EXPECT_EQUAL(BufferView(g_mockMsgBody["small.text"]), BufferView("small text file"), "Body should match");
        EXPECT_EQUAL(requestData.status, Http::Status::OK, "Request should be OK");
        EXPECT_EQUAL(HttpRequest.getParsingState(), Http::Request::COMPLETED, "request should be completed by now");

        TEST_PASSED_MSG(std::string("Valid body, multipart, small.text [" 
        + g_mockMsgBody["small.text"] + "], readBuf size: ") 
        + TestHelpers::to_string(readBufSize));
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
}

int main()
{
    TEST_HEADER("Http Request - Multipart Body");
    int testNumber = 1;
    chunkedReadBuffer(testNumber, 73);
    for (size_t i = 73; i < 123; ++i)
        chunkedReadBuffer(testNumber, i);
    chunkedReadBuffer2(testNumber, 1024);
    TEST_FOOTER;
    return 0;
}
