#include "../../HttpRequest.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

void regularBodyTests(int &testNumber)
{
    HttpRequest httpRequest;
    std::string requestData;

    TEST_HEADER("Http Request - Regular Body");

    TEST_HEADER("Passing Tests");

    ////////////////////////////////////////////

    TEST_INTRO(testNumber++);

    requestData =
        std::string("POST /test HTTP/1.1\r\n") +
        "Content-Length: 13\r\n" +
        "\r\n" +
        "Hello, World!";
    try
    {
        EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::OK, "Should pass");
        EXPECT_EQUAL(httpRequest.getBody(), "Hello, World!", "Body content should match");
        TEST_PASSED_MSG("Valid body with Content-Length");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    TEST_HEADER("Failing Tests");

    ////////////////////////////////////////////

    TEST_INTRO(testNumber++);

    requestData =
        std::string("POST /test HTTP/1.1\r\n") +
        "\r\n" +
        "Hello, World!";
    try
    {
        EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::BAD_REQUEST, "Should fail");
        TEST_PASSED_MSG("Missing Content-Length header");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    ////////////////////////////////////////////

    TEST_INTRO(testNumber++);

    requestData =
        std::string("POST /test HTTP/1.1\r\n") +
        "Content-Length: 20\r\n" +
        "\r\n" +
        "Hello, World!";
    try
    {
        EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::BAD_REQUEST, "Should fail");
        TEST_PASSED_MSG("Content-Length mismatch");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    ////////////////////////////////////////////

    TEST_INTRO(testNumber++);

    requestData =
        std::string("POST /test HTTP/1.1\r\n") +
        "Content-Length: invalid\r\n" +
        "\r\n" +
        "Hello, World!";
    try
    {
        EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::BAD_REQUEST, "Should fail");
        TEST_PASSED_MSG("Invalid Content-Length value");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    TEST_FOOTER;
}

void chunkedBodyTests(int &testNumber)
{
    HttpRequest httpRequest;
    std::string requestData;

    TEST_HEADER("Http Request - Chunked Body");

    TEST_HEADER("Passing Tests");

    ////////////////////////////////////////////

    TEST_INTRO(testNumber++);

    requestData =
        std::string("POST /test HTTP/1.1\r\n") +
        "Transfer-Encoding: chunked\r\n" +
        "\r\n" +
        "9\r\n" +
        "Wikipedia\r\n" +
        "3\r\n" +
        " in\r\n" +
        "7\r\n" +
        " chunks.\r\n" +
        "0\r\n" +
        "\r\n";
    try
    {
        EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::OK, "Should pass");
        EXPECT_EQUAL(httpRequest.getBody(), "Wikipedia in chunks.", "Body content should match");
        TEST_PASSED_MSG("Valid chunked body");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    ////////////////////////////////////////////

    TEST_INTRO(testNumber++);

    requestData =
        std::string("POST /test HTTP/1.1\r\n") +
        "Transfer-Encoding: chunked\r\n" +
        "\r\n" +
        "6;name=value\r\n" +
        "Hello,\r\n" +
        "7;ext=test\r\n" +
        " World!\r\n" +
        "0\r\n" +
        "\r\n";
    try
    {
        EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::OK, "Should pass");
        EXPECT_EQUAL(httpRequest.getBody(), "Hello, World!", "Body content should match");
        TEST_PASSED_MSG("Chunked body with extensions");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    TEST_HEADER("Failing Tests");

    ////////////////////////////////////////////

    TEST_INTRO(testNumber++);

    requestData =
        std::string("POST /test HTTP/1.1\r\n") +
        "Transfer-Encoding: chunked\r\n" +
        "\r\n" +
        "XYZ\r\n" +
        "Hello, World!\r\n" +
        "0\r\n" +
        "\r\n";
    try
    {
        EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::BAD_REQUEST, "Should fail");
        TEST_PASSED_MSG("Invalid chunk size");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    ////////////////////////////////////////////

    TEST_INTRO(testNumber++);

    requestData =
        std::string("POST /test HTTP/1.1\r\n") +
        "Transfer-Encoding: chunked\r\n" +
        "\r\n" +
        "6\r\n" +
        "Hello,\r\n" +
        "7\r\n" +
        " World!\r\n";
    try
    {
        EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::BAD_REQUEST, "Should fail");
        TEST_PASSED_MSG("Missing terminating chunk");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    ////////////////////////////////////////////

    TEST_INTRO(testNumber++);

    requestData =
        std::string("POST /test HTTP/1.1\r\n") +
        "Transfer-Encoding: chunked\r\n" +
        "\r\n" +
        "1000000\r\n" +
        std::string(10, 'A') + "\r\n" +
        "0\r\n" +
        "\r\n";
    try
    {
        EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::PAYLOAD_TOO_LARGE, "Should fail");
        TEST_PASSED_MSG("Oversized chunk");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    TEST_FOOTER;
}

int main()
{
    int testNumber = 1;

    std::cout << "Starting HTTP Request Body Parser Tests...\n\n";

    regularBodyTests(testNumber);
    chunkedBodyTests(testNumber);

    std::cout << "\nAll tests completed!\n";
    return 0;
}
