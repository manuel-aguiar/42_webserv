


# include "../../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../../../GenericUtils/Buffer/Buffer.hpp"
# include "../../../GenericUtils/BufferView/BufferView.hpp"
# include "Response.hpp"


void    testFullRead(int& testNumber)
{
    try
    {
        TEST_INTRO(testNumber++);
        
        const char* good = "Content-Type: text/html\n"
                            "\n"
                            "Hello World"
                            ;
        Buffer<1024> client;
        Buffer<1024> readBuffer;
        BufferView readView;
        Cgi::Response response(client);
    
        readBuffer.push(good);
        
        std::cout << readView.size() << "  '" << readView << "'" << std::endl;
        
        EXPECT_EQUAL(response.parse(readView), Cgi::Response::PASS, "failed to parse");

        TEST_PASSED_MSG("All tests passed!");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }
    
    
}