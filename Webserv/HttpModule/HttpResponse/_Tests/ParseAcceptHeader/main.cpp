

# include "../../../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../../../HttpDefinitions.hpp"
# include <vector>

struct MediaRange {
	std::string type;
	std::string subtype;
	float q;
};


extern std::vector<MediaRange> parseAcceptHeader(const std::string& header);
extern bool doesAccept(const std::vector<MediaRange>& types, const std::string& MediaRange);


# define EXAMPLE_HEADER1 "application/json, text/html; q=0.8, image/png; q=0.5"
# define EXAMPLE_HEADER2 "text/html, text/htm; q=0.8, image/png; q=0.5"
# define EXAMPLE_HEADER3 "application/json; q=1, image/png; q=0.5, text/*"
# define EXAMPLE_HEADER4 "application/json; q=0.8, image/png; q=0.5, */*"
# define EXAMPLE_HEADER5 "application/json; q=0.8, image/png; q=0.3, */*; q=0.1"
# define EXAMPLE_HEADER6 "application/json; q=0.8, image/png; q=0.5 */*; q=0"
# define EXAMPLE_HEADER7 "application"
# define EXAMPLE_HEADER8 "application; q=0.8"
# define EXAMPLE_HEADER9 ";"
# define EXAMPLE_HEADER10 "q=0.5"
# define EXAMPLE_HEADER11 "1232342/123145; a"


int main()
{
	int testNumber = 1;
	TEST_HEADER("Http Accept Header Parsing");
	std::vector<MediaRange> mr[11];
	mr[0] = parseAcceptHeader(EXAMPLE_HEADER1);
	mr[1] = parseAcceptHeader(EXAMPLE_HEADER2);
	mr[2] = parseAcceptHeader(EXAMPLE_HEADER3);
	mr[3] = parseAcceptHeader(EXAMPLE_HEADER4);
	mr[4] = parseAcceptHeader(EXAMPLE_HEADER5);
	mr[5] = parseAcceptHeader(EXAMPLE_HEADER6);
	mr[6] = parseAcceptHeader(EXAMPLE_HEADER7);
	mr[7] = parseAcceptHeader(EXAMPLE_HEADER8);
	mr[8] = parseAcceptHeader(EXAMPLE_HEADER9);
	mr[9] = parseAcceptHeader(EXAMPLE_HEADER10);
	mr[10] = parseAcceptHeader(EXAMPLE_HEADER11);


	//////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);

		EXPECT_EQUAL(doesAccept(mr[0], "text/html"), 1, EXAMPLE_HEADER1);
		TEST_PASSED_MSG("Accepts text/html");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	//////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);

		EXPECT_EQUAL(doesAccept(mr[1], "text/html"), 1, "Accepts text/html");
		TEST_PASSED_MSG("Accepts text/html");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);

		EXPECT_EQUAL(doesAccept(mr[2], "text/html"), 1, "Accepts text/html");
		TEST_PASSED_MSG("Accepts text/html");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);

		EXPECT_EQUAL(doesAccept(mr[3], "text/html"), 1, "Accepts text/html");
		TEST_PASSED_MSG("Accepts text/html");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);

		EXPECT_EQUAL(doesAccept(mr[4], "text/html"), 1, EXAMPLE_HEADER5);
		TEST_PASSED_MSG("Accepts text/html");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);

		EXPECT_EQUAL(doesAccept(mr[5], "text/html"), 0, "Desn't accept text/html");
		TEST_PASSED_MSG("Desn't accept text/html");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);

		EXPECT_EQUAL(doesAccept(mr[6], "text/html"), 0, "Desn't accept text/html");
		TEST_PASSED_MSG("Desn't accept text/html");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);

		EXPECT_EQUAL(doesAccept(mr[7], "text/html"), 0, "Desn't accept text/html");
		TEST_PASSED_MSG("Desn't accept text/html");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);

		EXPECT_EQUAL(doesAccept(mr[8], "text/html"), 0, "Desn't accept text/html");
		TEST_PASSED_MSG("Desn't accept text/html");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);

		EXPECT_EQUAL(doesAccept(mr[9], "text/html"), 0, "Desn't accept text/html");
		TEST_PASSED_MSG("Desn't accept text/html");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/////////////////////////////////////

	try
	{
		TEST_INTRO(testNumber++);

		EXPECT_EQUAL(doesAccept(mr[10], "text/html"), 0, "Desn't accept text/html");
		TEST_PASSED_MSG("Desn't accept text/html");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	TEST_FOOTER;
	return 0;
}
