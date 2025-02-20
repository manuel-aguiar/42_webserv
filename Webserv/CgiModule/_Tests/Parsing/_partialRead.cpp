

# include "../../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../../../GenericUtils/Buffer/Buffer.hpp"
# include "../../../GenericUtils/BufferView/BufferView.hpp"
# include "Response.hpp"

# include <cstring> //strlen

void testPartialRead(int& testNumber)
{
    try
    {
        TEST_INTRO(testNumber++);
        
        const char* good = "Status: 200 \n\n";

        Buffer<1024> client;
        Buffer<1024> readBuffer;
        Cgi::Response response(client);
        
        readBuffer.push(good, std::strlen(good) - 1);
        
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::Response::NEED_MORE_DATA, "should be correctly parsed without errors");
        EXPECT_EQUAL(response.getHeaders().size(), 1, "single header received");
        EXPECT_EQUAL(response.hasBody(), false, "no body");

        readBuffer.push(&good[std::strlen(good) - 1]);
        
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::Response::PASS, "should be correctly parsed without errors");
        EXPECT_EQUAL(response.getHeaders().size(), 1, "single header received");
        EXPECT_EQUAL(response.hasBody(), false, "no body");

        TEST_PASSED_MSG("Partial reads, Basic request, no body, should pass");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
//////////////////////////////////////////////////////
}