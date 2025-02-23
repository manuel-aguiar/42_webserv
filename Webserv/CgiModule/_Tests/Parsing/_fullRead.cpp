


# include "../../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../../../GenericUtils/Buffer/Buffer.hpp"
# include "../../../GenericUtils/BufferView/BufferView.hpp"
# include "../../HeaderData/HeaderData.hpp"


void    testFullRead(int& testNumber)
{
    try
    {
        TEST_INTRO(testNumber++);
        
        const char* good = "Location: tretas \n\n";

        Buffer<1024>    readBuffer;
        Cgi::HeaderData        response;
        
        readBuffer.push(good);
        
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::HeaderData::PASS, "should be correctly parsed without errors");
        EXPECT_EQUAL(response.getHeaders().size(), 1, "single header received");
        EXPECT_EQUAL(response.hasBody(), false, "no body");
        EXPECT_EQUAL(response.getStatusCode(), 200, "status code must be 200");


        TEST_PASSED_MSG("Basic request, no body, should pass");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
//////////////////////////////////////////////////////

    try
    {
        TEST_INTRO(testNumber++);
        
        const char* good = "Content-Type: tretas \n\n";

        Buffer<1024>    readBuffer;
        Cgi::HeaderData        response;
        
        readBuffer.push(good);
        
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::HeaderData::PASS, "needs more data, expects body");
        EXPECT_EQUAL(response.getHeaders().size(), 1, "single header received");
        EXPECT_EQUAL(response.hasBody(), true, "no body");
        EXPECT_EQUAL(response.getStatusCode(), 200, "status code must be 200");
        
        TEST_PASSED_MSG("Content-Type without message body");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
//////////////////////////////////////////////////////

    try
    {
        TEST_INTRO(testNumber++);
        
        const char* good = "SomeHeader: 200 \n";

        Buffer<1024> readBuffer;
        Cgi::HeaderData response;
        
        readBuffer.push(good);
        
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::HeaderData::NEED_MORE_DATA, "should be insufficient info");
        EXPECT_EQUAL(response.getHeaders().size(), 1, "single header received");
        EXPECT_EQUAL(response.hasBody(), false, "no body");
        EXPECT_EQUAL(response.getStatusCode(), -1, "status code must be -1");

        // buffer read again
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::HeaderData::FAIL, "no \n\n found, should fail");
        EXPECT_EQUAL(response.getStatusCode(), CGI_FAILURE, "status code must be -1");

        TEST_PASSED_MSG("Incomplete Request, should fail");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }    


//////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);
        
        const char* good = "SomeHeader: 200 \nSomeHeader: 200 \n\n";

        Buffer<1024> readBuffer;
        Cgi::HeaderData response;
        
        readBuffer.push(good);
        
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::HeaderData::FAIL, "should have found doubled headers");
        EXPECT_EQUAL(response.getHeaders().size(), 2, "two headers received, even though it failed");
        EXPECT_EQUAL(response.hasBody(), false, "no body");
        EXPECT_EQUAL(response.getStatusCode(), CGI_FAILURE, "status code must be -1");

        TEST_PASSED_MSG("Doubled headers, should fail");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }  

//////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);
        
        const char* good = "SomeHeader: 200 \nwtf: yes \n\nsomebody";

        Buffer<1024> readBuffer;
        Cgi::HeaderData response;
        
        readBuffer.push(good);
        
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::HeaderData::FAIL, "found body without content type header");
        EXPECT_EQUAL(response.getHeaders().size(), 2, "must be 2 headers");
        EXPECT_EQUAL(response.hasBody(), false, "no body");
        EXPECT_EQUAL(response.getStatusCode(), CGI_FAILURE, "status code must be -1");
        

        TEST_PASSED_MSG("Body without Content-Type, should fail");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }  



//////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);
        
        const char* good = "SomeHeader: 200 \nContent-Type: text/html\n\nsomebody";

        Buffer<1024> readBuffer;
        Cgi::HeaderData response;
        
        readBuffer.push(good);
        
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::HeaderData::PASS, "found body without content type header");
        EXPECT_EQUAL(response.getHeaders().size(), 2, "2 headers expected");
        EXPECT_EQUAL(response.hasBody(), true, "must be a body");
        EXPECT_EQUAL(response.getStatusCode(), CGI_SUCCESS, "status code must be 200");

        size_t bodyStart = readBuffer.view().find("\n\n", 2) + 2;
        BufferView body = BufferView(readBuffer.data() + bodyStart, readBuffer.size() - bodyStart);

        EXPECT_EQUAL(body, response.getTempBody(), "body must be 'somebody'");

        TEST_PASSED_MSG("Body with Content-Type, should pass");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    } 

//////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);
        
        const char* good = "\n";

        Buffer<1024> readBuffer;
        Cgi::HeaderData response;
        
        readBuffer.push(good);
        
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::HeaderData::FAIL, "empty string, should fail");
        EXPECT_EQUAL(response.getHeaders().size(), 0, "no headers");
        EXPECT_EQUAL(response.hasBody(), false, "no body");
        
        TEST_PASSED_MSG("string with a newline only, should fail");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    } 

//////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);
        
        const char* good = " \n";

        Buffer<1024> readBuffer;
        Cgi::HeaderData response;
        
        readBuffer.push(good);
        
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::HeaderData::FAIL, "empty string, should fail");
        EXPECT_EQUAL(response.getHeaders().size(), 0, "no headers");
        EXPECT_EQUAL(response.hasBody(), false, "no body");
        
        TEST_PASSED_MSG("string with a newline only, should fail");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    } 

//////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);
        
        const char* good = "\n\n";

        Buffer<1024> readBuffer;
        Cgi::HeaderData response;
        
        readBuffer.push(good);
        
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::HeaderData::FAIL, "empty string, should fail");
        EXPECT_EQUAL(response.getHeaders().size(), 0, "no headers");
        EXPECT_EQUAL(response.hasBody(), false, "no body");
        
        TEST_PASSED_MSG("string with a newline only, should fail");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    } 

}