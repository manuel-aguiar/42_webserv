#include "../../HttpRequest.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"
#include "../../../../GenericUtils/Buffer/Buffer.hpp"
#include "../../../../GenericUtils/BufferView/BufferView.hpp"
#include <sstream>
#include "../../../../GenericUtils/StringUtils/StringUtils.hpp"

void regularBodyTests(int &testNumber)
{
    TEST_HEADER("Http Request - Regular Body");

    {
        TEST_INTRO(testNumber++);
        Http::Request Request;
        Buffer<1024> buffer;
        buffer.push("POST /test HTTP/1.1\r\n"
                   "Content-Length: 13\r\n"
                   "\r\n"
                   "Hello, World!");

        try
        {
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::COMPLETED, "Should be completed");
            EXPECT_EQUAL(Request.getBody(), "Hello, World!", "Body content should match");
            TEST_PASSED_MSG("Valid body with Content-Length");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Request;
        Buffer<1024> buffer;
        buffer.push("POST /test HTTP/1.1\r\n"
                   "Content-Length: abc\r\n"
                   "\r\n");

        try
        {
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::ERROR, "Should be in error state");
            TEST_PASSED_MSG("Handled invalid Content-Length value");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Request;
        Buffer<1024> buffer;
        buffer.push("POST /test HTTP/1.1\r\n"
                   "Content-Length: 20\r\n"
                   "\r\n");

        try
        {
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::BODY, "Should be in body state");

            buffer.push("Hello");
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::BODY, "Should be in body state");

            buffer.push(", ");
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::BODY, "Should still be in body state");

            buffer.push("World");
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::BODY, "Should still be in body state");

            buffer.push("!!!!!!!!");
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::COMPLETED, "Should be completed");
            EXPECT_EQUAL(Request.getBody(), "Hello, World!!!!!!!!", "Final body should match");
            TEST_PASSED_MSG("Streamed body with multiple chunks");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Request;
        Buffer<1024> buffer;
        std::stringstream ss;
        ss << "POST /test HTTP/1.1\r\n"
           << "Content-Length: " << Http::HttpStandard::MAX_BODY_LENGTH + 1 << "\r\n"
           << "\r\n";
        buffer.push(ss.str());

        try
        {
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::ERROR, "Should be in error state");
            TEST_PASSED_MSG("Handled too large Content-Length");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Request;
        Buffer<1024> buffer;
        buffer.push("POST /test HTTP/1.1\r\n"
                   "Content-Length: 10\r\n"
                   "\r\n");

        try
        {
            Request.parse(buffer);
            buffer.push("Hello");
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::BODY, "Should be incomplete");

            buffer.push("WorldExtra");
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::ERROR, "Should be in error state");

            TEST_PASSED_MSG("Handled overflow data correctly");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Request;
        Buffer<1024> buffer;
        buffer.push("POST /test HTTP/1.1\r\n"
                   "Content-Length: " + StringUtils::intToStr(Http::HttpStandard::MAX_BODY_LENGTH + 1) + "\r\n"
                   "\r\n");

        try
        {
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::ERROR, "Should be in error state");

            TEST_PASSED_MSG("Handled too large Content-Length");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Request;
        Buffer<1024> buffer;
        buffer.push("POST /test HTTP/1.1\r\n"
                   "Content-Length: 10\r\n"
                   "\r\n");

        try
        {
            Request.parse(buffer);
            buffer.push("");
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::BODY, "Should still be incomplete");

            buffer.push("HelloWorld");
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::COMPLETED, "Should be completed");

            TEST_PASSED_MSG("Handled empty chunks correctly");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Request;
        std::string boundary = "------------------------12345";
        Buffer<1024> buffer;
        buffer.push("POST /upload HTTP/1.1\r\n"
                   "Content-Type: multipart/form-data; boundary=" + boundary + "\r\n"
                   "Content-Length: 213\r\n"
                   "\r\n"
                   "--" + boundary + "\r\n"
                   "Content-Disposition: form-data; name=\"field1\"\r\n"
                     "\r\n"
                     "value1\r\n"
                     "--" + boundary + "\r\n"
                     "Content-Disposition: form-data; name=\"field2\"\r\n"
                     "\r\n"
                     "value2\r\n"
                     "--" + boundary + "--\r\n");

        try
        {
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::COMPLETED, "Should be completed");
            EXPECT_EQUAL(Request.getBody().find("value1") != std::string::npos, true, "Should contain first field");
            EXPECT_EQUAL(Request.getBody().find("value2") != std::string::npos, true, "Should contain second field");
            TEST_PASSED_MSG("Valid multipart form-data request");
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
        Http::Request Request;
        Buffer<1024> buffer;
        buffer.push("POST /test HTTP/1.1\r\n"
                   "Transfer-Encoding: chunked\r\n"
                   "\r\n"
                   "5\r\n"
                   "Hello\r\n"
                   "5\r\n"
                   "World\r\n"
                   "0\r\n"
                   "\r\n");

        try
        {
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::COMPLETED, "Should be completed");
            EXPECT_EQUAL(Request.getBody(), "HelloWorld", "Body should be correctly assembled");
            TEST_PASSED_MSG("Valid chunked request");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Request;
        Buffer<1024> buffer;
        buffer.push("POST /test HTTP/1.1\r\n"
                   "Transfer-Encoding: chunked\r\n"
                   "\r\n");

        try
        {
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::BODY, "Should be incomplete");

            buffer.push("5\r\nHello\r\n");
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::BODY, "Should still be incomplete");

            buffer.push("5\r\nWorld\r\n");
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::BODY, "Should still be incomplete");

            buffer.push("0\r\n\r\n");
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::COMPLETED, "Should be completed");
            EXPECT_EQUAL(Request.getBody(), "HelloWorld", "Final body should match");
            TEST_PASSED_MSG("Streamed chunked request");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Request;
        Buffer<1024> buffer;
        buffer.push("POST /test HTTP/1.1\r\n"
                   "Transfer-Encoding: chunked\r\n"
                   "\r\n"
                   "invalid\r\n"
                   "World\r\n");

        try
        {
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::ERROR, "Should be in error state");
            TEST_PASSED_MSG("Invalid chunk size handled");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Request;
        Buffer<1024> buffer;
        buffer.push("POST /test HTTP/1.1\r\n"
                   "Transfer-Encoding: chunked\r\n"
                   "\r\n"
                   "5\r\n"
                   "Hello\r"
                   "5\r\n"
                   "World\r\n");

        try
        {
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::ERROR, "Should be in error state");
            TEST_PASSED_MSG("Invalid chunk format handled");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Request;
        Buffer<1024> buffer;
        buffer.push("POST /test HTTP/1.1\r\n"
                   "Transfer-Encoding: chunked\r\n"
                   "\r\n"
                   "5\r\n"
                   "Hello\r\n"
                   "0\r\n"
                   "Trailer: value\r\n"
                   "\r\n");

        try
        {
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::COMPLETED, "Should be completed");
            EXPECT_EQUAL(Request.getBody(), "Hello", "Body should be correctly assembled");
            TEST_PASSED_MSG("Chunked request with trailers");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }

    {
        TEST_INTRO(testNumber++);
        Http::Request Request;
        std::string boundary = "------------------------12345";
        Buffer<1024> buffer;
        buffer.push("POST /upload HTTP/1.1\r\n"
                   "Content-Type: multipart/form-data; boundary=" + boundary + "\r\n"
                   "Transfer-Encoding: chunked\r\n"
                   "\r\n");

        std::string part1 = "--" + boundary + "\r\n"
                           "Content-Disposition: form-data; name=\"field1\"\r\n"
                           "\r\n"
                           "value1\r\n";

        buffer.push(std::string() +
                      StringUtils::intToStr(part1.length()) + "\r\n" +
                      part1 + "\r\n");

        try
        {
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::BODY, "Should be in body state");

            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::BODY, "Should still be in body state");

            TEST_PASSED_MSG("Valid chunked multipart form-data request");
        }
        catch(const std::exception& e)
        {
            TEST_FAILED_MSG(e.what());
        }
    }
}

void multipartBodyTests(int &testNumber)
{
    TEST_HEADER("Http Request - Multipart Body");

    {
        TEST_INTRO(testNumber++);
        Http::Request Request;
        std::string boundary = "------------------------12345";
        Buffer<1024> buffer;
        buffer.push("POST /upload HTTP/1.1\r\n"
                   "Content-Type: multipart/form-data; boundary=" + boundary + "\r\n"
                   "Content-Length: 100\r\n"
                   "\r\n"
                   "--" + boundary + "\r\n"
                   "Content-Disposition: form-data; name=\"field1\"\r\n"
                   "\r\n"
                   "incomplete");

        try
        {
            Request.parse(buffer);
            EXPECT_EQUAL(Request.getParsingState(), Http::Request::BODY, "Should stay in body state");
            TEST_PASSED_MSG("Partial multipart form-data handling");
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
    multipartBodyTests(testNumber);
    return 0;
}
