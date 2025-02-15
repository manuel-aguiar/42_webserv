#include "../../HttpRequest.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

void regularBodyTests(int &testNumber)
{
    HttpRequest httpRequest;
    std::string requestData;

    TEST_HEADER("Http Request - Regular Body");

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
}

int main()
{
    int testNumber = 1;

    std::cout << "Starting HTTP Request Body Parser Tests...\n\n";

    regularBodyTests(testNumber);

    std::cout << "\nAll tests completed!\n";
    return 0;
}
