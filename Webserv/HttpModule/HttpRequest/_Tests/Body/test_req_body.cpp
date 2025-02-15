#include "../../HttpRequest.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

void regularBodyTests(int &testNumber)
{
    TEST_HEADER("Http Request - Regular Body");

    {
        TEST_INTRO(testNumber++);
        HttpRequest httpRequest;
        std::string requestData =
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
    }

    {
        TEST_INTRO(testNumber++);
        HttpRequest httpRequest;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\nContent-Length: abc\r\n\r\n");

        try
        {
            // Test invalid Content-Length value
            EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::BAD_REQUEST, "Invalid Content-Length should fail");
            EXPECT_EQUAL(httpRequest.getParsingState(), HttpRequest::ERROR, "Should be in error state");

            TEST_PASSED_MSG("Handled invalid Content-Length value");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        HttpRequest httpRequest;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\n") +
            "Content-Length: -1\r\n" +
            "\r\n";
        try
        {
            EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::BAD_REQUEST, "Should fail");
            EXPECT_EQUAL(httpRequest.getBody(), "", "Body content should match");
            TEST_PASSED_MSG("Invalid Content-Length");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        HttpRequest httpRequest;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\nContent-Length: 13\r\n\r\n");

        try
        {
            EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::OK, "Should pass");
            EXPECT_EQUAL(httpRequest.parse(std::string("Hello, World!")), (int)Http::Status::OK, "Should pass");
            EXPECT_EQUAL(httpRequest.getBody(), "Hello, World!", "Body content should match");
            TEST_PASSED_MSG("Valid body with Content-Length");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        HttpRequest httpRequest;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\nContent-Length: 20\r\n\r\n");

        try
        {
            // Send data in multiple chunks
            EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::OK, "Initial parse should pass");
            EXPECT_EQUAL(httpRequest.parse("Hello"), (int)Http::Status::OK, "First chunk should pass");
            EXPECT_EQUAL(httpRequest.getParsingState(), HttpRequest::INCOMPLETE, "Should be incomplete");

            EXPECT_EQUAL(httpRequest.parse(", "), (int)Http::Status::OK, "Second chunk should pass");
            EXPECT_EQUAL(httpRequest.getParsingState(), HttpRequest::INCOMPLETE, "Should still be incomplete");

            EXPECT_EQUAL(httpRequest.parse("World"), (int)Http::Status::OK, "Third chunk should pass");
            EXPECT_EQUAL(httpRequest.getParsingState(), HttpRequest::INCOMPLETE, "Should still be incomplete");

            EXPECT_EQUAL(httpRequest.parse("!!!!!!!!"), (int)Http::Status::OK, "Final chunk should pass");
            EXPECT_EQUAL(httpRequest.getParsingState(), HttpRequest::COMPLETED, "Should be completed");

            EXPECT_EQUAL(httpRequest.getBody(), "Hello, World!!!!!!!!", "Final body should match");
            TEST_PASSED_MSG("Streamed body with multiple chunks");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        HttpRequest httpRequest;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\nContent-Length: 10\r\n\r\n");

        try
        {
            // Test sending more data than Content-Length
            EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::OK, "Initial parse should pass");
            EXPECT_EQUAL(httpRequest.parse("Hello"), (int)Http::Status::OK, "First chunk should pass");
            EXPECT_EQUAL(httpRequest.getParsingState(), HttpRequest::INCOMPLETE, "Should be incomplete");

            EXPECT_EQUAL(httpRequest.parse("WorldExtra"), (int)Http::Status::BAD_REQUEST, "Exceeding Content-Length should fail");
            EXPECT_EQUAL(httpRequest.getParsingState(), HttpRequest::ERROR, "Should be in error state");

            TEST_PASSED_MSG("Handled overflow data correctly");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        HttpRequest httpRequest;
        // Test with max body length + 1
        std::stringstream ss;
        ss << "POST /test HTTP/1.1\r\nContent-Length: " << (Http::HttpStandard::MAX_BODY_LENGTH + 1) << "\r\n\r\n";

        try
        {
            EXPECT_EQUAL(httpRequest.parse(ss.str()), (int)Http::Status::PAYLOAD_TOO_LARGE, "Should reject too large Content-Length");
            EXPECT_EQUAL(httpRequest.getParsingState(), HttpRequest::ERROR, "Should be in error state");

            TEST_PASSED_MSG("Handled too large Content-Length");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        HttpRequest httpRequest;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\nContent-Length: 10\r\n\r\n");

        try
        {
            // Test empty chunks
            EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::OK, "Initial parse should pass");
            EXPECT_EQUAL(httpRequest.parse(""), (int)Http::Status::OK, "Empty chunk should be accepted");
            EXPECT_EQUAL(httpRequest.getParsingState(), HttpRequest::INCOMPLETE, "Should still be incomplete");

            EXPECT_EQUAL(httpRequest.parse("HelloWorld"), (int)Http::Status::OK, "Complete data should pass");
            EXPECT_EQUAL(httpRequest.getParsingState(), HttpRequest::COMPLETED, "Should be completed");

            TEST_PASSED_MSG("Handled empty chunks correctly");
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
