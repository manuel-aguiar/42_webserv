

#ifndef ASSERT_EQUAL

	#ifndef NDEBUG
		#include <iostream>
		#include <cstdlib> // for abort
		#include "../../TestHelpers/TestHelpers.h"
		#define ASSERT_EQUAL(actual, expected, message)                                		 				\
			do	{                                                                           				\
				if ((actual) != (expected))                                             					\
				{                                                                       					\
					std::cerr 	<< "\n\n"                                               					\
								<< "\tABORTING PROGRAM\n"                               					\
								<< TEST_ERROR_MSG((actual), (expected), (message))  						\
								<< std::endl;                                          						\
					std::abort();                                                     						\
				}                                                                   						\
      		}	while (0)

	#else

		#define ASSERT_EQUAL(actual, expected, message) ((void)0)

	#endif

#endif