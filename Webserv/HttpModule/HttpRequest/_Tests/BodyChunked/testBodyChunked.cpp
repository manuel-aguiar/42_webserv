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
extern Buffer<1024> g_mockMsgBody;

void    chunkedReadBuffer(int& testNumber, size_t readBufSize)
{
    TEST_INTRO(testNumber++);

    ServerContext context;

    Http::Request   HttpRequest(context);
    Http::Response  HttpResponse(context);

    const Http::RequestData& requestData = HttpRequest.getData();

    Buffer<1024> bufferRequest;

    HttpRequest.setResponse(HttpResponse);
    HttpResponse.setRequest(HttpRequest);

    HeapBuffer buffer(readBufSize);
    HttpRequest.setBuffer_ReadFd(buffer, Ws::FD_NONE);

    std::string requestheader = "POST /test HTTP/1.1\r\n"
        "Transfer-Encoding: chunked\r\n"
        "Content-Type: pota\rto\r\n"
        "\r\n";
    std::string requestBodyChunked = 
        "4\r\n"
        "Wiki\r\n"
        "5\r\n"
        "pedia\r\n"
        "D\r\n"
        " in \rchunks.\r\r\n"
        "0\r\n"
        "\r\n";
    
    std::string requestBodyTranslated = "Wikipedia in \rchunks.\r";

    std::string request = requestheader + requestBodyChunked;

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
        }
        //std::cout << "requestData.status = " << requestData.status << std::endl;
        EXPECT_EQUAL(g_mockMsgBody.view(), BufferView(requestBodyTranslated), "Body should match");

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
    chunkedReadBuffer(testNumber, 29);
    for (size_t i = 28; i < 101; ++i)
        chunkedReadBuffer(testNumber, i);
    chunkedReadBuffer(testNumber, 1024);
    TEST_FOOTER;
    return 0;
}
