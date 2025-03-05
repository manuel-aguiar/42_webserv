

#include "../../HttpRequest.hpp"
#include "../../../../ServerContext/ServerContext.hpp"
#include "../../../HttpResponse/HttpResponse.hpp"
#include "../../../../ServerContext/ServerContext.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

void validRequestLineTests(int &testNumber)
{
    TEST_HEADER("Http Request - Valid Request Lines");
    ServerContext context;
    // Test 1: Simple GET request
    TEST_INTRO(testNumber++);
    {
        Http::Request request(context);
        std::string requestData =
            std::string("GET /index.html HTTP/1.1\r\n") +
            "Host: localhost\r\n\r\n";

        Buffer<1024> buffer;
        buffer.push(requestData.c_str(), requestData.size());
        try
        {
            request.setBuffer_ReadFd(buffer, Ws::FD_NONE);
            request.parse();
            EXPECT_EQUAL(request.getParsingState(), (int)Http::Request::COMPLETED, "Parsing state should be COMPLETED");
            
            const Http::RequestData& data = request.getData();
            
            EXPECT_EQUAL(data.status,                     (int)Http::Status::OK,          "Status should be OK");
            EXPECT_EQUAL(BufferView(data.method),         BufferView("GET"),              "Method should be GET");
            EXPECT_EQUAL(BufferView(data.uri),            BufferView("/index.html"),      "URI should match");
            EXPECT_EQUAL(BufferView(data.httpVersion),    BufferView("HTTP/1.1"),         "Version should match");
            EXPECT_EQUAL(BufferView(data.path),           BufferView("/index.html"),      "Path should match");
            EXPECT_EQUAL(BufferView(data.queryString),    BufferView(""),                 "Query string should be empty");
            EXPECT_EQUAL(BufferView(data.fragment),       BufferView(""),                 "Fragment should be empty");



            TEST_PASSED_MSG("Valid simple GET request");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    // Test 2: GET request with query parameters and fragment
    TEST_INTRO(testNumber++);
    {
        Http::Request request(context);
        std::string requestData =
            std::string("GET /search?q=test&page=1#section1 HTTP/1.1\r\n") +
            "Host: localhost\r\n\r\n";
        
        Buffer<1024> buffer;
        buffer.push(requestData.c_str(), requestData.size());
        try
        {
            request.setBuffer_ReadFd(buffer, Ws::FD_NONE);
            request.parse();

            const Http::RequestData& data = request.getData();

            EXPECT_EQUAL(data.status, (int)Http::Status::OK, "Should pass");
            EXPECT_EQUAL(request.getPath(), "/search", "Path should match");
            EXPECT_EQUAL(request.getQueryString(), "q=test&page=1", "Query string should match");
            EXPECT_EQUAL(request.getFragment(), "section1", "Fragment should match");
            TEST_PASSED_MSG("Valid GET request with query parameters and fragment");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }
}

void invalidRequestLineTests(int &testNumber)
{
    TEST_HEADER("Http Request - Invalid Request Lines");

    ServerContext context;

    // Test 1: Invalid method
    TEST_INTRO(testNumber++);
    {
        Http::Request Request(context);
        std::string requestData =
            std::string("INVALID /index.html HTTP/1.1\r\n") +
            "Host: localhost\r\n\r\n";
        Buffer<1024> buffer;
        buffer.push(requestData.c_str(), requestData.size());
        try
        {
            Request.setBuffer_ReadFd(buffer, Ws::FD_NONE);
            Request.parse();
            const Http::RequestData& data = Request.getData();
            EXPECT_EQUAL(data.status, (int)Http::Status::METHOD_NOT_ALLOWED, "Should fail");
            TEST_PASSED_MSG("Invalid method detection");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    // Test 2: URI too long
    TEST_INTRO(testNumber++);
    {
        Http::Request Request(context);
        std::string longUri = "/";
        for (int i = 0; i < 2048; ++i) {
            longUri += "a";
        }
        std::string requestData =
            std::string("GET ") + longUri + " HTTP/1.1\r\n" +
            "Host: localhost\r\n\r\n";
        Buffer<4096> buffer;
        buffer.push(requestData.c_str(), requestData.size());

        try
        {
            Request.setBuffer_ReadFd(buffer, Ws::FD_NONE);
            Request.parse();
            const Http::RequestData& data = Request.getData();

            EXPECT_EQUAL(data.status, (int)Http::Status::URI_TOO_LONG, "Should fail");
            TEST_PASSED_MSG("URI too long detection");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }
}

void encodedUriTests(int &testNumber)
{
    TEST_HEADER("Http Request - URI Encoding Tests");

    ServerContext context;
    // Test 1: Valid encoded URI
    TEST_INTRO(testNumber++);
    {
        Http::Request Request(context);
        std::string requestData =
            std::string("GET /search%20path/file%2B1.html?name=%4A%6F%68%6E&title=Hello%20World%21 HTTP/1.1\r\n") +
            "Host: localhost\r\n\r\n";
        Buffer<1024> buffer;
        buffer.push(requestData.c_str(), requestData.size());
        try
        {
            Request.setBuffer_ReadFd(buffer, Ws::FD_NONE);
            Request.parse();
            const Http::RequestData& data = Request.getData();

            EXPECT_EQUAL(data.status, (int)Http::Status::OK, "Should pass");
            EXPECT_EQUAL(Request.getPath(), "/search path/file+1.html", "Decoded path should match");
            EXPECT_EQUAL(Request.getQueryString(), "name=%4A%6F%68%6E&title=Hello%20World%21", "Raw query string should match");
            TEST_PASSED_MSG("Valid encoded URI parsing");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    // Test 2: Invalid URI encoding
    TEST_INTRO(testNumber++);
    {
        Http::Request Request(context);
        std::string requestData =
            std::string("GET /test%2path?param=%XX HTTP/1.1\r\n") +
            "Host: localhost\r\n\r\n";
        
        Buffer<1024> buffer;
        buffer.push(requestData.c_str(), requestData.size());

        try
        {
            Request.setBuffer_ReadFd(buffer, Ws::FD_NONE);
            Request.parse();
            const Http::RequestData& data = Request.getData();
            EXPECT_EQUAL(data.status, (int)Http::Status::BAD_REQUEST, "Should fail");
            TEST_PASSED_MSG("Invalid URI encoding detection");
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
    validRequestLineTests(testNumber);
    invalidRequestLineTests(testNumber);
    encodedUriTests(testNumber);
    return 0;
}
