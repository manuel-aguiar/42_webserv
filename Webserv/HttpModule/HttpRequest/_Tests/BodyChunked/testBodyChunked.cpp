#include "../../HttpRequest.hpp"
#include "../../../HttpResponse/HttpResponse.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"
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
    Http::Response  response(context);
    const Http::RequestData& requestData = HttpRequest.getData();

    Buffer<1024> bufferRequest;

    HttpRequest.setResponse(response);

    HeapBuffer buffer(readBufSize);

    std::string requestheader = "POST /test HTTP/1.1\r\n"
        "Transfer-Encoding: chunked\r\n"
        "\r\n";
    std::string requestBodyChunked = 
        "4\r\n"
        "Wiki\r\n"
        "5\r\n"
        "pedia\r\n"
        "B\r\n"
        " in chunks.\r\n"
        "0\r\n"
        "\r\n";
    
    std::string requestBodyTranslated = "Wikipedia in chunks.";

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
            //////std::cout << "bufferRequest.size() = " << bufferRequest.size() << std::endl;
            HttpRequest.parse(buffer);
        }
        //std::cout << "requestData.status = " << requestData.status << std::endl;
        EXPECT_EQUAL(g_mockMsgBody.view(), BufferView(requestBodyTranslated), "Body should match");

        TEST_PASSED_MSG(std::string("Valid body, chuncked, readBuf size: ") 
        + TestHelpers::to_string(readBufSize)  + " '" + requestBodyTranslated + "'");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
}


int main()
{
    int testNumber = 1;
    for (size_t i = 28; i < 101; ++i)
        chunkedReadBuffer(testNumber, i);
    chunkedReadBuffer(testNumber, 1024);
    return 0;
}
