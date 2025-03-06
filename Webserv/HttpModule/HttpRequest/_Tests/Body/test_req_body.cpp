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
            HttpRequest.parse();
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


void regularBodyTests(int &testNumber)
{
    TEST_HEADER("Http Request - Regular Body");
    ServerContext context;
    Buffer<2048> buffer;

    
    {
        TEST_INTRO(testNumber++);
        Http::Request   HttpRequest(context);
        Http::Response  HttpResponse(context);


        HttpRequest.setResponse(HttpResponse);
        HttpResponse.setRequest(HttpRequest);

        Buffer<33> buffer;

        HttpRequest.setBuffer_ReadFd(buffer, Ws::FD_NONE);

        std::string requestheader = "POST /test HTTP/1.1\r\n"
            "Content-Length: 104\r\n"
            "\r\n";
        std::string requestBody =
            "Hello, World!"
            "Hello, World!"
            "Hello, World!"
            "Hello, World!"
            "Hello, World!"
            "Hello, World!"
            "Hello, World!"
            "Hello, World!";
        
        std::string request = requestheader + requestBody;

        g_mockMsgBody.clear();

        try
        {
            while (request.size())
            {
                int thisPush = buffer.available() < request.size() ? buffer.available() : request.size();
                buffer.push(request.c_str(), thisPush);
                request.erase(0, thisPush);

                HttpRequest.parse();
            }

            EXPECT_EQUAL(g_mockMsgBody.view(), BufferView(requestBody), "Body should match");

            TEST_PASSED_MSG("Valid body with Content-Length");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }
/////////////////////////////////////////////////////////////////////////////////////////
    {
        TEST_INTRO(testNumber++);
        Http::Request   HttpRequest(context);
        Http::Response  HttpResponse(context);


        HttpRequest.setResponse(HttpResponse);
        HttpResponse.setRequest(HttpRequest);
        Buffer<1024> buffer;

        HttpRequest.setBuffer_ReadFd(buffer, Ws::FD_NONE);

        //using a buffer as well for the request, to allow all types of bytes
        Buffer<1024> bufferRequest;


        std::string requestHeader = "POST /test HTTP/1.1\r\n"
            "Content-Length: 16\r\n"
            "\r\n";

        //random message body bytes to simulate binary (4 integers, 16 bytes );
        int requestBody[] = {2123123123, 1231231231, -111111111, -222222222};
        
        bufferRequest.push(requestHeader.c_str(), requestHeader.size());
        bufferRequest.push((char*)&requestBody, sizeof(requestBody));

        g_mockMsgBody.clear();

        try
        {
            while (bufferRequest.size())
            {
                int thisPush = buffer.available() < bufferRequest.size() ? buffer.available() : bufferRequest.size();
                buffer.push(BufferView(bufferRequest.data(), thisPush));
                bufferRequest.truncatePush(BufferView(bufferRequest.data() + thisPush, bufferRequest.size() - thisPush));
                HttpRequest.parse();
            }

            EXPECT_EQUAL(g_mockMsgBody.view(), BufferView((char*)&requestBody, sizeof(requestBody)), "Body should match");
            
            TEST_PASSED_MSG(std::string("Valid body with weird bytes: ") + g_mockMsgBody.view().to_string());
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }
/////////////////////////////////////////////////////////////////////////////////////////////////
    {
        TEST_INTRO(testNumber++);
        Http::Request   HttpRequest(context);
        Http::Response  HttpResponse(context);


        HttpRequest.setResponse(HttpResponse);
        HttpResponse.setRequest(HttpRequest);
        Buffer<33> buffer;

        HttpRequest.setBuffer_ReadFd(buffer, Ws::FD_NONE);
        //using a buffer as well for the request, to allow all types of bytes
        Buffer<1024> bufferRequest;


        std::string requestHeader = "POST /test HTTP/1.1\r\n"
            "Content-Length: 16\r\n"
            "\r\n";

        //random message body bytes to simulate binary (4 integers, 16 bytes );
        int requestBody[] = {2123123123, 0, 0, -222222222};
        
        bufferRequest.push(requestHeader.c_str(), requestHeader.size());
        bufferRequest.push((char*)&requestBody, sizeof(requestBody));

        g_mockMsgBody.clear();

        try
        {
            while (bufferRequest.size())
            {
                int thisPush = buffer.available() < bufferRequest.size() ? buffer.available() : bufferRequest.size();
                buffer.push(BufferView(bufferRequest.data(), thisPush));
                bufferRequest.truncatePush(BufferView(bufferRequest.data() + thisPush, bufferRequest.size() - thisPush));
                HttpRequest.parse();
            }

            EXPECT_EQUAL(g_mockMsgBody.view(), BufferView((char*)&requestBody, sizeof(requestBody)), "Body should match");
            
            TEST_PASSED_MSG(std::string("Valid body with nulls: ") + g_mockMsgBody.view().to_string());
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }
}

int main()
{
    int testNumber = 1;
    regularBodyTests(testNumber);
    return 0;
}
