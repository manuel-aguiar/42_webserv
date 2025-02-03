
#ifndef TESTHELPERS_H

# define TESTHELPERS_H

#include <string>
#include <sstream>
#include <stdexcept>

/*
	Just some ugly colors for the tests
*/
# define TEST_CLR_BLUE "\033[34m"
# define TEST_CLR_RED "\033[31m"
# define TEST_CLR_GREY "\033[30m"
# define TEST_CLR_GREEN "\033[32m"
# define TEST_CLR_BROWN "\033[1;38;5;130m"
# define TEST_CLR_RESET "\033[0m"

# include <iostream>

///////////////////////////////////////////////////////////////////////////
/*
	Place this at the beginning of the test main, to signal the tests are beginning
	pass it your class name for instance
*/
# define TEST_HEADER(message)	\
	do	{						\
		std::cout << TEST_CLR_BROWN << "\n*************** [" << (message) << "]  Tests ***************" << TEST_CLR_RESET << std::endl;	\
	}	while (0)

///////////////////////////////////////////////////////////////////////////

/*
	Place this at the end of your test main to signal the end of tests for a class
*/
# define TEST_FOOTER	\
	do	{						\
		std::cout << TEST_CLR_BROWN << "**************************************************\n" << TEST_CLR_RESET << std::endl;	\
	}	while (0)

///////////////////////////////////////////////////////////////////////////

/*
	Place this at the beginning of each test. THe macro can take any argument but ideally
	pass it the number of your test
*/
# define TEST_INTRO(testNumber)	\
	do 	{						\
		std::cout << TEST_CLR_BLUE << "TEST " << (testNumber) << ": " << TEST_CLR_RESET;	\
	} 	while (0)

///////////////////////////////////////////////////////////////////////////

/*
	Passing test, no custom message
*/
# define TEST_PASSED	\
	do	{						\
		std::cout << TEST_CLR_GREEN << "\tPASSED" << TEST_CLR_RESET << std::endl;			\
	}	while (0)

///////////////////////////////////////////////////////////////////////////

/*
	Failing test, no custom message
*/
# define TEST_FAILED \
	do	{						\
		std::cout << TEST_CLR_RED << "\tFAILED" << TEST_CLR_RESET << std::endl;			\
	}	while (0)

///////////////////////////////////////////////////////////////////////////

/*
	Passing test, adding a custom message
*/
# define TEST_PASSED_MSG(message)	\
	do	{						\
		std::cout << TEST_CLR_GREEN << "\tPASSED" << TEST_CLR_GREY << " (" << (message) << ")"  << TEST_CLR_RESET << std::endl;			\
	}	while (0)

///////////////////////////////////////////////////////////////////////////

/*
	Failing test, adding a custom message
*/
# define TEST_FAILED_MSG(message) \
	do	{						\
		std::cout << TEST_CLR_RED << "\tFAILED" << TEST_CLR_GREY << " (" << (message) << ")"  << TEST_CLR_RESET << std::endl;			\
	}	while (0)

namespace TestHelpers
{
	/*
		Just a helper function
	*/
	template <typename T>
	std::string to_string(const T& value)
	{
		std::ostringstream oss;
		oss << value;
		return (oss.str());
	}


	#ifndef TEST_ERROR_MSG
			#include <sstream>
			/*
				Generates an error message (returns a const char*). This macro is called by EXPECT_EQUAL
				and also ASSERT_EQUAL. You can call it directly if you have chained tests and need
				to cleanup resources before throwing an exception.

				But it is most a helper.
				Args:
					actual - the result of what your testing
					expected - what you expect the outcome to be
					message - a custom message you want to display
			*/
			#define TEST_ERROR_MSG(actual, expected, message) (										\
				{                    														     	\
					std::ostringstream oss;                                                      	\
					oss << "\t----------------------------------\n"                              	\
						<< "\tError:     '" << (message) << "'\n"                                	\
						<< "\tResult:    '" << (actual)   << "' [" << #actual   << "]\n"         	\
						<< "\tExpected:  '" << (expected) << "' [" << #expected << "]\n"         	\
						<< "\t----------------------------------\n"                              	\
						<< "\tFile:       " << __FILE__  << ":" << __LINE__ << "\n"              	\
						<< "\tLine:       " << __LINE__  << "\n"                                 	\
						<< "\tFunction:   " << __FUNCTION__ << "\n"                              	\
						<< "\t----------------------------------\n";                             	\
					oss.str();                                                                   	\
				}																					\
			)
	#endif
	
	#ifndef EXPECT_EQUAL
		#include <sstream>
		/*
			Compares to values (actual vs expected) and throws an exception if they are not equal.
			It uses TEST_ERROR_MSG to generate the error message.
			Args:
				actual - the result of what your testing
				expected - what you expect the outcome to be
				message - a custom message you want to display in case of failing equality (actual != expected)
		*/
		#define EXPECT_EQUAL(actual, expected, message)                               			\
			do	{                                                                           	\
				if ((actual) != (expected))														\
					throw std::logic_error(std::string("\n\n") + TEST_ERROR_MSG(actual, expected, message));	\
			}	while (0)																	
		
	#endif

}

#endif