#include "../../HttpRequest.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

void validRequestLineTests(int &testNumber)
{
    TEST_HEADER("Http Request - Valid Request Lines");

    // Test 1: Simple GET request
    TEST_INTRO(testNumber++);
    {
        Http::Request Request;
        std::string requestData =
            std::string("GET /index.html HTTP/1.1\r\n") +
            "Host: localhost\r\n\r\n";
        try
        {
            EXPECT_EQUAL(Request.parse(requestData), (int)Http::Status::OK, "Should pass");
            EXPECT_EQUAL(Request.getMethod(), "GET", "Method should be GET");
            EXPECT_EQUAL(Request.getUri(), "/index.html", "URI should match");
            EXPECT_EQUAL(Request.getHttpVersion(), "HTTP/1.1", "Version should match");
            TEST_PASSED_MSG("Valid simple GET request");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    // Test 2: GET request with query parameters
    TEST_INTRO(testNumber++);
    {
        Http::Request Request;
        std::string requestData =
            std::string("GET /search?q=test&page=1#section1 HTTP/1.1\r\n") +
            "Host: localhost\r\n\r\n";
        try
        {
            EXPECT_EQUAL(Request.parse(requestData), (int)Http::Status::OK, "Should pass");
            const std::map<std::string, std::string>& components = Request.getUriComponents();
            EXPECT_EQUAL(components.at("path"), "/search", "Path component should match");
            EXPECT_EQUAL(components.at("q"), "test", "Query parameter 'q' should match");
            EXPECT_EQUAL(components.at("page"), "1", "Query parameter 'page' should match");
            TEST_PASSED_MSG("Valid GET request with query parameters");
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

    // Test 1: Invalid method
    TEST_INTRO(testNumber++);
    {
        Http::Request Request;
        std::string requestData =
            std::string("INVALID /index.html HTTP/1.1\r\n") +
            "Host: localhost\r\n\r\n";
        try
        {
            EXPECT_EQUAL(Request.parse(requestData), (int)Http::Status::METHOD_NOT_ALLOWED, "Should fail");
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
        Http::Request Request;
        std::string longUri = "/";
        for (int i = 0; i < 2048; ++i) {
            longUri += "a";
        }
        std::string requestData =
            std::string("GET ") + longUri + " HTTP/1.1\r\n" +
            "Host: localhost\r\n\r\n";
        try
        {
            EXPECT_EQUAL(Request.parse(requestData), (int)Http::Status::URI_TOO_LONG, "Should fail");
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

    // Test 1: Valid encoded URI
    TEST_INTRO(testNumber++);
    {
        Http::Request Request;
        std::string requestData =
            std::string("GET /search%20path/file%2B1.html?name=%4A%6F%68%6E&title=Hello%20World%21 HTTP/1.1\r\n") +
            "Host: localhost\r\n\r\n";
        try
        {
            EXPECT_EQUAL(Request.parse(requestData), (int)Http::Status::OK, "Should pass");
            const std::map<std::string, std::string>& components = Request.getUriComponents();
            EXPECT_EQUAL(components.at("path"), "/search path/file+1.html", "Decoded path should match");
            EXPECT_EQUAL(components.at("name"), "John", "Decoded name parameter should match");
            EXPECT_EQUAL(components.at("title"), "Hello World!", "Decoded title parameter should match");
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
        Http::Request Request;
        std::string requestData =
            std::string("GET /test%2path?param=%XX HTTP/1.1\r\n") +
            "Host: localhost\r\n\r\n";
        try
        {
            EXPECT_EQUAL(Request.parse(requestData), (int)Http::Status::BAD_REQUEST, "Should fail");
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
