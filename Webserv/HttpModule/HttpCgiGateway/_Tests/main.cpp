

# include "../../../CgiModule/CgiNamespace.h"
# include "../../../../Toolkit/TestHelpers/TestHelpers.h"

# include <vector>

extern bool checkForbiddenHeaders(const std::vector<Cgi::Header>& headers);
extern bool isHeaderIgnored(const Cgi::Header& header);

int main(void)
{
    int testNumber = 1;
    TEST_HEADER("Http cgi header validation");

    try
    {
        TEST_INTRO(testNumber);

        std::vector<Cgi::Header> headers;
        
        headers.push_back((Cgi::Header){"Connection", "close"});
        
        EXPECT_EQUAL(checkForbiddenHeaders(headers), false, "Connection header is forbidden");

        TEST_PASSED_MSG("invalid connection header");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }

    try
    {
        TEST_INTRO(testNumber);

        std::vector<Cgi::Header> headers;
        
        headers.push_back((Cgi::Header){"Content-Length", "cenas"});
        headers.push_back((Cgi::Header){"something", "alright"});

        EXPECT_EQUAL(checkForbiddenHeaders(headers), true, "ignored header but valid");

        int count = 0;
        for (size_t i = 0; i < headers.size(); ++i)
        {
            if (!isHeaderIgnored(headers[i]))
                count++;
        }

        EXPECT_EQUAL(count, 1, "ignored header was removed");

        TEST_PASSED_MSG("ignored header in the middle");
    }
    catch(const std::exception& e)
    {
        TEST_FAILED_MSG(e.what());
    }


    TEST_FOOTER;
    return (0);
}