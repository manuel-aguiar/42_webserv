
#ifndef TESTHELPERS_H

# define TESTHELPERS_H

#include <string>
#include <sstream>
#include <stdexcept>


namespace TestHelpers
{

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