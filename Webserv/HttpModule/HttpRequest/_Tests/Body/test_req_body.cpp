#include "../../Http::Request.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

void regularBodyTests(int &testNumber)
{
    TEST_HEADER("Http Request - Regular Body");

    {
        TEST_INTRO(testNumber++);
        Http::Request Http::Request;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\n") +
            "Content-Length: 13\r\n" +
            "\r\n" +
            "Hello, World!";
        try
        {
            EXPECT_EQUAL(Http::Request.parse(requestData), (int)Http::Status::OK, "Should pass");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::COMPLETED, "Should be completed");
            EXPECT_EQUAL(Http::Request.getBody(), "Hello, World!", "Body content should match");
            TEST_PASSED_MSG("Valid body with Content-Length");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Http::Request;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\nContent-Length: abc\r\n\r\n");

        try
        {
            // Test invalid Content-Length value
            EXPECT_EQUAL(Http::Request.parse(requestData), (int)Http::Status::BAD_REQUEST, "Invalid Content-Length should fail");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::ERROR, "Should be in error state");

            TEST_PASSED_MSG("Handled invalid Content-Length value");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Http::Request;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\n") +
            "Content-Length: -1\r\n" +
            "\r\n";
        try
        {
            EXPECT_EQUAL(Http::Request.parse(requestData), (int)Http::Status::BAD_REQUEST, "Should fail");
            EXPECT_EQUAL(Http::Request.getBody(), "", "Body content should match");
            TEST_PASSED_MSG("Invalid Content-Length");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Http::Request;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\nContent-Length: 13\r\n\r\n");

        try
        {
            EXPECT_EQUAL(Http::Request.parse(requestData), (int)Http::Status::OK, "Should pass");
            EXPECT_EQUAL(Http::Request.parse(std::string("Hello, World!")), (int)Http::Status::OK, "Should pass");
            EXPECT_EQUAL(Http::Request.getBody(), "Hello, World!", "Body content should match");
            TEST_PASSED_MSG("Valid body with Content-Length");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Http::Request;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\nContent-Length: 20\r\n\r\n");

        try
        {
            // Send data in multiple chunks
            EXPECT_EQUAL(Http::Request.parse(requestData), (int)Http::Status::OK, "Initial parse should pass");
            EXPECT_EQUAL(Http::Request.parse("Hello"), (int)Http::Status::OK, "First chunk should pass");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::INCOMPLETE, "Should be incomplete");

            EXPECT_EQUAL(Http::Request.parse(", "), (int)Http::Status::OK, "Second chunk should pass");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::INCOMPLETE, "Should still be incomplete");

            EXPECT_EQUAL(Http::Request.parse("World"), (int)Http::Status::OK, "Third chunk should pass");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::INCOMPLETE, "Should still be incomplete");

            EXPECT_EQUAL(Http::Request.parse("!!!!!!!!"), (int)Http::Status::OK, "Final chunk should pass");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::COMPLETED, "Should be completed");

            EXPECT_EQUAL(Http::Request.getBody(), "Hello, World!!!!!!!!", "Final body should match");
            TEST_PASSED_MSG("Streamed body with multiple chunks");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Http::Request;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\nContent-Length: 10\r\n\r\n");

        try
        {
            // Test sending more data than Content-Length
            EXPECT_EQUAL(Http::Request.parse(requestData), (int)Http::Status::OK, "Initial parse should pass");
            EXPECT_EQUAL(Http::Request.parse("Hello"), (int)Http::Status::OK, "First chunk should pass");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::INCOMPLETE, "Should be incomplete");

            EXPECT_EQUAL(Http::Request.parse("WorldExtra"), (int)Http::Status::BAD_REQUEST, "Exceeding Content-Length should fail");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::ERROR, "Should be in error state");

            TEST_PASSED_MSG("Handled overflow data correctly");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Http::Request;
        // Test with max body length + 1
        std::stringstream ss;
        ss << "POST /test HTTP/1.1\r\nContent-Length: " << (Http::HttpStandard::MAX_BODY_LENGTH + 1) << "\r\n\r\n";

        try
        {
            EXPECT_EQUAL(Http::Request.parse(ss.str()), (int)Http::Status::PAYLOAD_TOO_LARGE, "Should reject too large Content-Length");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::ERROR, "Should be in error state");

            TEST_PASSED_MSG("Handled too large Content-Length");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Http::Request;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\nContent-Length: 10\r\n\r\n");

        try
        {
            // Test empty chunks
            EXPECT_EQUAL(Http::Request.parse(requestData), (int)Http::Status::OK, "Initial parse should pass");
            EXPECT_EQUAL(Http::Request.parse(""), (int)Http::Status::OK, "Empty chunk should be accepted");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::INCOMPLETE, "Should still be incomplete");

            EXPECT_EQUAL(Http::Request.parse("HelloWorld"), (int)Http::Status::OK, "Complete data should pass");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::COMPLETED, "Should be completed");

            TEST_PASSED_MSG("Handled empty chunks correctly");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }
}

void chunkedBodyTests(int &testNumber)
{
    TEST_HEADER("Http Request - Chunked Body");

    {
        TEST_INTRO(testNumber++);
        Http::Request Http::Request;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\n") +
            "Transfer-Encoding: chunked\r\n" +
            "\r\n" +
            "5\r\n"
            "Hello\r\n"
            "5\r\n"
            "World\r\n"
            "0\r\n"
            "\r\n";

        try
        {
            EXPECT_EQUAL(Http::Request.parse(requestData), (int)Http::Status::OK, "Valid chunked request should pass");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::COMPLETED, "Should be completed");
            EXPECT_EQUAL(Http::Request.getBody(), "HelloWorld", "Body should be correctly assembled");
            TEST_PASSED_MSG("Valid chunked request");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Http::Request;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\n") +
            "Transfer-Encoding: chunked\r\n" +
            "\r\n";

        try
        {
            // Test streaming chunks
            EXPECT_EQUAL(Http::Request.parse(requestData), (int)Http::Status::OK, "Initial headers should pass");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::INCOMPLETE, "Should be incomplete");

            EXPECT_EQUAL(Http::Request.parse("5\r\nHello\r\n"), (int)Http::Status::OK, "First chunk should pass");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::INCOMPLETE, "Should still be incomplete");

            EXPECT_EQUAL(Http::Request.parse("5\r\nWorld\r\n"), (int)Http::Status::OK, "Second chunk should pass");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::INCOMPLETE, "Should still be incomplete");

            EXPECT_EQUAL(Http::Request.parse("0\r\n\r\n"), (int)Http::Status::OK, "Final chunk should pass");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::COMPLETED, "Should be completed");

            EXPECT_EQUAL(Http::Request.getBody(), "HelloWorld", "Final body should match");
            TEST_PASSED_MSG("Streamed chunked request");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Http::Request;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\n") +
            "Transfer-Encoding: chunked\r\n" +
            "\r\n" +
            "invalid\r\n"
            "World\r\n";

        try
        {
            EXPECT_EQUAL(Http::Request.parse(requestData), (int)Http::Status::BAD_REQUEST, "Invalid chunk size should fail");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::ERROR, "Should be in error state");
            TEST_PASSED_MSG("Invalid chunk size handled");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Http::Request;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\n") +
            "Transfer-Encoding: chunked\r\n" +
            "\r\n" +
            "5\r\n"
            "Hello\r" // Missing \n in chunk data delimiter
            "5\r\n"
            "World\r\n";

        try
        {
            EXPECT_EQUAL(Http::Request.parse(requestData), (int)Http::Status::BAD_REQUEST, "Invalid chunk format should fail");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::ERROR, "Should be in error state");
            TEST_PASSED_MSG("Invalid chunk format handled");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Http::Request;
        // Test with chunk size exceeding max allowed
        std::stringstream ss;
        ss << std::hex << (Http::HttpStandard::MAX_CHUNK_SIZE + 1) << "\r\n";
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\n") +
            "Transfer-Encoding: chunked\r\n" +
            "\r\n" +
            ss.str();

        try
        {
            EXPECT_EQUAL(Http::Request.parse(requestData), (int)Http::Status::PAYLOAD_TOO_LARGE, "Too large chunk should fail");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::ERROR, "Should be in error state");
            TEST_PASSED_MSG("Too large chunk size handled");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Http::Request;
        std::string requestData =
            std::string("POST /test HTTP/1.1\r\n") +
            "Transfer-Encoding: chunked\r\n" +
            "\r\n" +
            "5\r\n"
            "Hello\r\n"
            "0\r\n"
            "Trailer: value\r\n" // trailing headers
            "\r\n";

        try
        {
            EXPECT_EQUAL(Http::Request.parse(requestData), (int)Http::Status::OK, "Chunked request with trailers should pass");
            EXPECT_EQUAL(Http::Request.getParsingState(), Http::Request::COMPLETED, "Should be completed");
            EXPECT_EQUAL(Http::Request.getBody(), "Hello", "Body should be correctly assembled");
            TEST_PASSED_MSG("Chunked request with trailers");
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
    chunkedBodyTests(testNumber);
    return 0;
}
