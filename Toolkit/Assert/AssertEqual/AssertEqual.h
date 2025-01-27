

#ifndef ASSERT_EQUAL

	#ifndef NDEBUG
		#include <iostream>
		#include <cstdlib> // for abort
		
		#define ASSERT_EQUAL(actual, expected, message)                                		 				\
			do                                                                          					\
			{                                                                           					\
				if ((actual) != (expected))                                             					\
				{                                                                       					\
					std::cerr << "\n\n"                                               						\
								<< "\tABORTING PROGRAM\n"                               					\
								<< "\t----------------------------------\n"             					\
								<< "\tError:     '" << (message) 	<< "'\n"             					\
								<< "\tResult:    '" << (actual) 	<< "' [" << #actual 	<< "]\n"        \
								<< "\tExpected:  '" << (expected) 	<< "' [" << #expected 	<< "]\n" 		\
								<< "\t----------------------------------\n"             					\
								<< "\tFile:       " << __FILE__ 	<< ":" << __LINE__ << "\n" 				\
								<< "\tLine:       " << __LINE__ 	<< "\n"               					\
								<< "\tFunction:   " << __FUNCTION__ << "\n"           						\
								<< "\t----------------------------------\n"             					\
								<< std::endl;                                          						\
					std::abort();                                                     						\
				}                                                                   						\
      } while (0)

	#else

		#define ASSERT_EQUAL(actual, expected, message) ((void)0)

	#endif

#endif