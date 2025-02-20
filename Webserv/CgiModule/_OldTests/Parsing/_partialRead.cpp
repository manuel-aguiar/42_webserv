

# include "../../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../../../GenericUtils/Buffer/Buffer.hpp"
# include "../../../GenericUtils/BufferView/BufferView.hpp"
# include "../../Response/Response.hpp"

# include <cstring> //strlen

void testPartialBreakPoint(int& testNumber, const char* message, int breakpoint)
{
    try
    {
        TEST_INTRO(testNumber++);
        
        Buffer<1024> readBuffer;
        Response response;
        
        readBuffer.push(message, breakpoint);
        
        EXPECT_EQUAL(response.parse(readBuffer), Response::NEED_MORE_DATA, "should be correctly parsed without errors");

        readBuffer.push(&message[breakpoint]);
        
        EXPECT_EQUAL(response.parse(readBuffer), Response::PASS, "should be correctly parsed without errors");
        EXPECT_EQUAL(response.getHeaders().size(), 1, "single header received");
        EXPECT_EQUAL(response.hasBody(), false, "no body");

        TEST_PASSED_MSG("Breakpoint, Partial reads, Basic request, no body, should pass");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
}

void testPartialRead(int& testNumber)
{
    const char* good = "Status: 200 \n\n";
    testPartialBreakPoint(testNumber, good, 4);
    testPartialBreakPoint(testNumber, good, std::strlen(good) - 1);
}