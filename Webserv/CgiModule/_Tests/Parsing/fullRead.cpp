


# include "../../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../../../GenericUtils/Buffer/Buffer.hpp"
# include "../../../GenericUtils/BufferView/BufferView.hpp"
# include "Response.hpp"


void    testFullRead(int& testNumber)
{
    try
    {
        TEST_INTRO(testNumber++);
        
        const char* good = "Status: 200 \n\n";

        Buffer<1024> client;
        Buffer<1024> readBuffer;
        Cgi::Response response(client);
        
        readBuffer.push(good);
        
        //std::cout << readView.size() << "  '" << readView << "'" << std::endl;
        
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::Response::PASS, "should be correctly parsed without errors");
        EXPECT_EQUAL(response.getHeaders().size(), 1, "single header received");
        EXPECT_EQUAL(response.hasBody(), false, "no body");


        TEST_PASSED_MSG("Basic request, no body");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
//////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);
        
        const char* good = "Status: 200 \n";

        Buffer<1024> client;
        Buffer<1024> readBuffer;
        Cgi::Response response(client);
        
        readBuffer.push(good);
        
        //std::cout << readView.size() << "  '" << readView << "'" << std::endl;
        
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::Response::KEEP_READING, "should be insufficient info");
        EXPECT_EQUAL(response.getHeaders().size(), 1, "single header received");
        EXPECT_EQUAL(response.hasBody(), false, "no body");

        // buffer read again
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::Response::FAIL, "no \n\n found, should fail");


        TEST_PASSED_MSG("Incomplete Request");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }    


//////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);
        
        const char* good = "Status: 200 \nStatus: 200 \n\n";

        Buffer<1024> client;
        Buffer<1024> readBuffer;
        Cgi::Response response(client);
        
        readBuffer.push(good);
        
        //std::cout << readView.size() << "  '" << readView << "'" << std::endl;
        
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::Response::FAIL, "should have found doubled headers");
        EXPECT_EQUAL(response.getHeaders().size(), 1, "single header received");
        EXPECT_EQUAL(response.hasBody(), false, "no body");


        TEST_PASSED_MSG("Doubled headers");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }  

//////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);
        
        const char* good = "Status: 200 \nwtf: yes \n\nsomebody";

        Buffer<1024> client;
        Buffer<1024> readBuffer;
        Cgi::Response response(client);
        
        readBuffer.push(good);
        
        //std::cout << readView.size() << "  '" << readView << "'" << std::endl;
        
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::Response::FAIL, "found body without content type header");
        EXPECT_EQUAL(response.getHeaders().size(), 2, "must be 2 headers");
        EXPECT_EQUAL(response.hasBody(), false, "no body");


        TEST_PASSED_MSG("Body without Content-Type");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }  

//////////////////////////////////////////////////////
    try
    {
        TEST_INTRO(testNumber++);
        
        const char* good = "Status: 200 \nStatus: 200 \n\nsomebody";

        Buffer<1024> client;
        Buffer<1024> readBuffer;
        Cgi::Response response(client);
        
        readBuffer.push(good);
        
        //std::cout << readView.size() << "  '" << readView << "'" << std::endl;
        
        EXPECT_EQUAL(response.parse(readBuffer), Cgi::Response::FAIL, "found body without content type header");
        EXPECT_EQUAL(response.getHeaders().size(), 1, "single header received");
        EXPECT_EQUAL(response.hasBody(), false, "no body");


        TEST_PASSED_MSG("Body with Content-Type");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    } 

}