
#ifndef TESTHELPERS_H

# define TESTHELPERS_H

#include <string>
#include <sstream>
#include <stdexcept>

# define TEST_CLR_BLUE "\033[34m"
# define TEST_CLR_RED "\033[31m"
# define TEST_CLR_GREEN "\033[32m"
# define TEST_CLR_RESET "\033[0m"

# include <iostream>

# define TEST_INTRO(testNumber)	\
	do 	{						\
		std::cout << TEST_CLR_BLUE << "TEST " << (testNumber) << ": " << TEST_CLR_RESET;	\
	} 	while (0)

# define TEST_PASSED	\
	do	{						\
		std::cout << TEST_CLR_GREEN << "\tPASSED" << TEST_CLR_RESET << std::endl;			\
	}	while (0)

# define TEST_FAILED \
	do	{						\
		std::cout << TEST_CLR_RED << "\tFAILED" << TEST_CLR_RESET << std::endl;			\
	}	while (0)

# define TEST_PASSED_MSG(message)	\
	do	{						\
		std::cout << TEST_CLR_GREEN << "\tPASSED" << TEST_CLR_RESET << " (" << (message) << ")" << std::endl;			\
	}	while (0)

# define TEST_FAILED_MSG(message) \
	do	{						\
		std::cout << TEST_CLR_RED << "\tFAILED" << TEST_CLR_RESET << " (" << (message) << ")" << std::endl;			\
	}	while (0)

namespace TestHelpers
{

	template <typename T>
	std::string to_string(const T& value)
	{
		std::ostringstream oss;
		oss << value;
		return (oss.str());
	}

	#ifndef TEST_ERROR_MSG
			#include <sstream>
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
		#define EXPECT_EQUAL(actual, expected, message)                               			\
			do	{                                                                           	\
				if ((actual) != (expected))														\
					throw std::logic_error(TEST_ERROR_MSG((actual), (expected), (message)));	\
			}	while (0)																	
		
	#endif

}
#endif